#include <WiFi.h>
#include <AsyncTCP.h>
#include <LittleFS.h>
#include "globals.h"
#include "sse.h"
#include "serialParser.h"
#include "timer.h"
#include "ble-uart.h"
#include "tcp-uart.h"
#include "config.h"
#include "web_config.h"
#include "wifi_mgt.h"
#include "updater.h"

#include <soc/efuse_reg.h>
#include <esp_efuse.h>

#define SPIFFS LittleFS

uint32_t loopCounter = 0;

void setup() 
{  
  pinMode(GPIO_NUM_8, INPUT_PULLDOWN);
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);   // Workaround for https://github.com/espressif/arduino-esp32/issues/6983. Removing this will cause the loop to run VERY slowly unless the serial monitor is connected
  initConfig();
  initTimers();
  initBLE();
  initWiFi();

  delay(1000);
  Serial.println("Connection Type: " + String(config.getUChar("connection_type")));

  if( (config.getUChar("connection_type") == CONNECTION_TYPE_TUNERSTUDIO) )
  {
    initTCP();
  }

  if(updatesPending())
  {
    //When updates are pending, only show the minimum pages
    server.on("/updateStatus", HTTP_GET, [](AsyncWebServerRequest *request) {
          request->send(200, "text/json", update_progress_json(request));
        });
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", updateInProgressPage());
      });
  }
  else
  {
    if( (config.getUChar("connection_type") == CONNECTION_TYPE_WIFI) && (updatesPending() == false) )
    {
      //Init file system
      if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
      }
      initSSE();
      initSerialData();
      
      //Init the web server
      // Web Server Root URL
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
      });
      
      server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        String jsonOutput;
        serializeJson(readings_JSON, jsonOutput);
        //request->send(200, "text/json", JSON.stringify(readings_JSON));
        request->send(200, "text/json", jsonOutput.c_str());
      });

      server.serveStatic("/", SPIFFS, "/");

    }
    else
    {
      //If not using the web dash then the root URL will produce the config page
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", webConfigRequest(request));
      });
    }

    server.on(WEB_CONFIG_URL, HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", webConfigRequest(request));
      });

    server.on(WEB_CONFIG_URL, HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", webConfigPOSTRequest(request));
      });

    //Updates the firmware AND data from remote URLs
    server.on(UPDATE_REMOTE_URL, HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", saveRemoteFW_URLs(request));
        delay(1000); //Wait 1 second to allow the page to be sent before restarting
        ESP.restart();
      });
    //Scan the wifi networks and return them as JSON
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/json", scanWifi(request));
      });

    server.on(UPDATE_DATA_UPLOAD_URL, HTTP_POST, [](AsyncWebServerRequest *request) {
        //This runs when the uplaod is completed
        partitionUploadComplete(request);
      },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        //This runs each time a new chunk is received
        partitionUploadChunk(request, filename, index, data, len, final, U_SPIFFS);
      }
      );
    server.on(UPDATE_FW_UPLOAD_URL, HTTP_POST, [](AsyncWebServerRequest *request) {
      //This runs when the uplaod is completed
      partitionUploadComplete(request);
      },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
      //This runs each time a new chunk is received
      partitionUploadChunk(request, filename, index, data, len, final, U_FLASH);
      }
      );  
  }
  

  // Start server
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");
  DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache");
  DefaultHeaders::Instance().addHeader("X-Accel-Buffering", "no");

  server.begin();
  
  updateFromRemote(); //Check for pending remote firmware updates

  //By default the ESP32-C3 will output a bunch of diag messages on bootup over UART. 
  //This messes up the secondary serial on the Speeduino so these bootup messages are disabled.
  //The only way to achieve this is to burn the eFuse value, so this is a one way operation (ie it cannot be undone)
  if(REG_GET_FIELD(EFUSE_RD_REPEAT_DATA3_REG, EFUSE_UART_PRINT_CONTROL) == 0) //Check whether eFUSE is in its original state
  {
    //Burn eFuse value to disable UART messages on bootup
    esp_efuse_set_rom_log_scheme(ESP_EFUSE_ROM_LOG_ALWAYS_OFF); //New eFuse value will be 3
  }

  Serial_ECU.begin(115200);
  delay(500);
  while(Serial_ECU.available()) { Serial_ECU.read(); } //In case unit has restarted and ECU is still sending data over UART
}

void loop() 
{
  LOOP_TIMER = TIMER_mask;
  
  //delay(1000);
  loopCounter++;

  if(BIT_CHECK(LOOP_TIMER, BIT_TIMER_30HZ)) //30 hertz
  {
    BIT_CLEAR(TIMER_mask, BIT_TIMER_30HZ);

    if(config.getUChar("connection_type") == CONNECTION_TYPE_WIFI)
    {
      if(Serial_ECU.available())
      {
        parseFixedSerialData();
      }
      if(serialECURequestQueueSize < 2) { requestSerialData(); }
      notifyClients();
    }
  }

  if(config.getUChar("connection_type") == CONNECTION_TYPE_BLE)
  {
    if(Serial_ECU.available())
    {
      Serial.print("Received message back from ECU: ");

      std::string tempMsg;
      uint16_t len = Serial_ECU.available();
      for(uint16_t i = 0; i < len; i++)
      {
        tempMsg.append(1, (char)Serial_ECU.read());
      }
      Serial.println(tempMsg.c_str());
      SendMessageBLE(tempMsg);
    }
  }

  if(BIT_CHECK(LOOP_TIMER, BIT_TIMER_1HZ)) //1 hertz
  {
    BIT_CLEAR(TIMER_mask, BIT_TIMER_1HZ);
    
    if(Serial_ECU)
    {
      if(config.getUChar("connection_type") == CONNECTION_TYPE_WIFI)
      {
        sendPing();
        Serial.print("Notifications Sent: ");
        Serial.println(notificationsSent);

        if(serialECURequestQueueSize > 3)
        {
          //Not getting responses from ECU
          sendNoDataMessage(); //Alert clients that no data is available
          /*          Serial_ECU.flush();
          Serial_ECU.end();
          Serial.println("Serial connection lost. Reconnecting");
          Serial_ECU.begin(115200);
          serialECURequestQueueSize = 0;
          */

        }
      }
      else if(config.getUChar("connection_type") == CONNECTION_TYPE_TUNERSTUDIO)
      {
        if(numTCPClients > 0)
        {
          Serial.print("TunerStudio Requests received: ");
          Serial.println(TCPrequestsReceived);
        }
      }
    }
    else 
    { 
      Serial.println("No serial connection available. Retrying");
      Serial_ECU.begin(115200); 
    } //Retry serial connection
  }
}
