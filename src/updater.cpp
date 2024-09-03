/*
This file contains routines for remotely updating the LittleFS partition that contains the HTML frontend files
*/
#include "updater.h"
#include "esp_partition.h"
#include "timer.h"

#include <HTTPUpdate.h>
#include <HTTPClient.h>
#include <Update.h>

#include "../lib/esp_ghota/include/semver.h"
#include "../lib/esp_ghota/include/esp_ghota.h"

char org_name[] = "speeduino";
char repo_name[] = "AirBear";

//Load a data partition from a remote URL
String dataPartitionDownload(AsyncWebServerRequest *request, uint8_t partitionType)
{
  String resultPage = "";
  if (request->hasParam("update_url", true)) 
  {
    HTTPClient client;
    t_httpUpdate_return ret = httpUpdate.updateSpiffs(client, request->getParam("update_url", true)->value());
    switch (ret) 
    {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
    }
    if (ret == HTTP_UPDATE_OK) 
    {
      
    }
  }
  return resultPage;

}

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

void firmwarePartitionDownload(AsyncWebServerRequest *request)
{
  if (request->hasParam("update_url", true)) 
  {
    HTTPClient client; 
    Serial.println("Update sketch...");
    t_httpUpdate_return ret = httpUpdate.update(client, request->getParam("update_url", true)->value());

    switch (ret) 
    {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
    }
  }
}

void updateFromRelease()
{
  ghota_config_t ghconfig = {
    {.filenamematch = "AirBear-esp32-c3.bin"}, // Glob Pattern to match against the Firmware file
    {.storagenamematch = "AirBear-Dash-Data.bin"}, // Glob Pattern to match against the storage firmware file
    {.storagepartitionname = "spiffs"}, // Update the storage partition
    };
    ghconfig.orgname = org_name;
    ghconfig.reponame = repo_name;

    static const char* TAG = "main";
    ghota_client_handle_t *ghota_client = ghota_init(&ghconfig);
    if (ghota_client == NULL) {
        ESP_LOGE(TAG, "ghota_client_init failed");
        return;
    }
}