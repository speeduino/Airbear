#include "wifi_mgt.h"
#include "globals.h"
#include "config.h"
#include <WiFi.h>
#include <ESPmDNS.h>

#include "lwip/api.h"
#include "lwip/tcp.h"

// Initialize WiFi
void initWiFi() 
{
  WiFi.mode(WIFI_STA);

  String ssid = config.getString("ssid", ""); 
  String password = config.getString("wpa-psk", "");

  if (ssid == "")
  {
    Serial.println("No values saved for ssid or password. Starting AP mode.");
    startAPMode();
  }
  else
  {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");

    uint8_t timeoutCounter = 0;
    while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print('.');
      delay(1000);
      timeoutCounter++;

      if(timeoutCounter > MAX_WIFI_TIMEOUT)
      {
        Serial.println("Could not connect to WiFi. Starting AP mode.");
        startAPMode();
        break;
      }
    }
  }

  // Set an mDNS name
  if (!MDNS.begin("speeduino")) { Serial.println("Error setting up MDNS responder!"); }
  else
  {
    mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
    mdns_service_instance_name_set("_http", "_tcp", "Speeduino Web Dash");
  }

  Serial.println(WiFi.localIP());
}

void startAPMode()
{
  //Do AP mode stuff here
  WiFi.mode(WIFI_AP);

  String ssid = config.getString("ap-ssid", ""); 
  String password = config.getString("ap-psk", "");

  if(ssid != "") { WiFi.softAP(ssid, password); }
  else 
  { 
    //If no config details have been saved for the AP mode, use a default name and no password
    WiFi.softAP("Speeduino Dash", "");
    config.putString("ap-ssid", "Speeduino Dash");
    config.putString("ap-psk", "");
  } 
}
