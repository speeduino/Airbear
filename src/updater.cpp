/*
This file contains routines for remotely updating the LittleFS partition that contains the HTML frontend files
*/
#include "updater.h"
#include "esp_partition.h"
#include "timer.h"
#include "config.h"
#include "timer.h"
#include "static/static_js.h"
#include "static/static_html.h"
#include "static/static_css.h"

#include <WiFi.h>
#include <HTTPUpdate.h>
#include <Update.h>

uint32_t updateSize = 0;
uint32_t updateProgress = 0;
String  updateComment = "";

//Load a data partition from an uploaded file
void partitionUploadChunk(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final, uint8_t partitionType)
{
  //Upload handler chunks in data
  if(!index)
  { // if index == 0 then this is the first frame of data
    Serial.printf("UploadStart: %s\n", filename.c_str());
    Serial.println("Stopping all timers");
    stopTimers();
    Serial.setDebugOutput(true);
    
    //Determine type of update based on the partition type
    if(partitionType == U_SPIFFS)
    {
      //Data update
      //The update size SHOULD match the existing data partition size
      //If not then someone has created the LittleFS image with the wrong size
      const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
      if(!Update.begin(partition->size, U_SPIFFS)) { Update.printError(Serial); } 
    }
    else if(partitionType == U_FLASH)
    {
      //Firmware update
      //The update size SHOULD match the existing data partition size
      //If not then someone has created the LittleFS image with the wrong size
      const esp_partition_t *partition = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
      if(!Update.begin(partition->size, U_FLASH)) { Update.printError(Serial); }
    }

  }

  //Write chunked data to the free sketch space
  if(Update.write(data, len) != len) { Update.printError(Serial); }
  
  if(final)
  { // if the final flag is set then this is the last frame of data
    if(Update.end(true)){ //true to set the size to the current progress
        Serial.printf("Update Success: %u B\nRebooting...\n", index+len);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
  }
}

void partitionUploadComplete(AsyncWebServerRequest *request)
{
  String responseHTML = "";
  responseHTML += "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Update Complete</title><meta http-equiv=\"refresh\" content=\"5;url=/\" /></head>";
  responseHTML += "<body><center><h1>";
  if(!Update.hasError()) { responseHTML += "Update completed successfully."; }
  else { responseHTML += "Update failed. Error: " + Update.getError(); }
  responseHTML += "</h1><br/>Reloading in 5 seconds...</center></body></html>";

  AsyncWebServerResponse *response = request->beginResponse(200, "text/html", responseHTML);
  response->addHeader("Connection", "close");
  response->addHeader("Access-Control-Allow-Origin", "*");
  //restartRequired = true;  // Tell the main loop to restart the ESP
  request->send(response);
  delay(1000);
  ESP.restart();
}

String saveRemoteFW_URLs(AsyncWebServerRequest *request)
{

  if (request->hasParam("newFW_url", true)) 
  {
    config.putString("newFW_url", request->getParam("newFW_url", true)->value());
  }
  if (request->hasParam("newData_url", true)) 
  {
    config.putString("newData_url", request->getParam("newData_url", true)->value());
  }

  return updateInProgressPage();
}

String updateInProgressPage()
{
  //Create the updates page
  String updatePage = staticHTML_head();
  updatePage += staticJS_updates();
  updatePage += staticCSS_config();
  updatePage += "</head><body onLoad=\"updateProgress()\">";
  updatePage += "<div class=\"container\">";
  updatePage += staticHTML_logo();
  updatePage += "<input id=\"tab-1\" type=\"radio\" name=\"tabs\" class=\"tabs\" checked>";
  updatePage += "<label for=\"tab-1\">Update Progress</label>";
  updatePage += "<div class=\"content\">";

  updatePage += "<div id=\"content-1\">";
  updatePage += "<strong>Current Status: </strong><span id=\"updateStatus\">Preparing to update</span><br/>";
  updatePage += "<strong>Update Size: </strong><span id=\"updateSize\"></span><br/>";
  updatePage += "<strong>Downloaded: </strong><span id=\"updateComplete\"></span><br/>";
  updatePage += "<strong>Update Completion: </strong><span id=\"updatePercent\"></span>%<div/>";

  updatePage += "<br/></div></div></body></html>";

  return updatePage;
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
  updateSize = total;
  updateProgress = cur;
}

bool updatesPending()
{
  return ( (config.getString("newFW_url", "") != "") || (config.getString("newData_url", "") != "") );
}

String update_progress_json(AsyncWebServerRequest *request)
{
  JsonDocument json;
  json["updateSize"] = updateSize;
  json["updateProgress"] = updateProgress;
  json["updateStatus"] = updateComment;

  String output;
  serializeJson(json, output);
  return output;
}

void updateFromRemote()
{
  String newFW_url = config.getString("newFW_url", ""); 
  String newData_url = config.getString("newData_url", "");

  if(newFW_url != "")
  {
    WiFiClient client;
    
    Serial.print("Attempting to update FW from: ");
    Serial.println(newFW_url);  //Blank the config values so that it doesn't get stuck in a fail loop

    httpUpdate.onProgress(update_progress);
    config.putString("newFW_url", ""); 
    updateComment = "Updating Firmware (Step 1 of 2)";
    t_httpUpdate_return ret = httpUpdate.update(client, newFW_url);
    
    switch (ret) 
    {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s \n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK:
        config.putString("newFW_url", ""); 
        Serial.println("HTTP_UPDATE_OK"); 
        break;
    }
  }
  if(newData_url != "")
  {
    WiFiClient client;
    
    Serial.print("Attempting to update Data from: ");
    Serial.println(newData_url);

    httpUpdate.onProgress(update_progress);
    updateComment = "Updating Filesystem (Step 2 of 2)";
    t_httpUpdate_return ret = httpUpdate.updateSpiffs(client, newData_url);
    config.putString("newData_url", "");

    switch (ret) 
    {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s \n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK: 
        Serial.println("HTTP_UPDATE_OK"); 
        config.putString("newData_url", "");
        delay(1300); //Need to wait at least 1 update cycle (1100ms) so that the frontend knows the update is complete
        ESP.restart();
        break;
    }
  }
}