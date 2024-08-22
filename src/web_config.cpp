#include "web_config.h"
#include "updater.h"
#include "config.h"

String webConfigRequest(AsyncWebServerRequest *request)
{
  String response = "";
  response += "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Speeduino Web Config</title>";
  //response += "<script>function _(e){return document.getElementById(e)}function uploadFile(e,n){var a=_(e).files[0],o=new FormData;o.append(e,a);var t=new XMLHttpRequest;t.upload.addEventListener(\"progress\",progressHandler,!1),t.open(\"POST\",n),t.send(o)}function progressHandler(e){_(\"loaded_n_total\").innerHTML=\"Uploaded \"+e.loaded+\" bytes of \"+e.total;var n=e.loaded/e.total*100;_(\"progressBar\").value=Math.round(n)}</script>";
  response += "<link rel=\"stylesheet\" href=\"/css/config.css\" />";
  response += "</head>";
  response += "<body>";
  response += "<div class=\"container\">";
  response += "<input id=\"tab-1\" type=\"radio\" name=\"tabs\" class=\"tabs\" checked>";
  response += "<label for=\"tab-1\">Config</label>";
  response += "<input id=\"tab-2\" type=\"radio\" name=\"tabs\" class=\"tabs\">";
  response += "<label for=\"tab-2\">Wifi</label>";
  response += "<input id=\"tab-3\" type=\"radio\" name=\"tabs\" class=\"tabs\">";
  response += "<label for=\"tab-3\">Update</label>";
  
  response += "<div class=\"content\">";
  //------------------------------------ Config ------------------------------------
  response += "<div id=\"content-1\">";
  response += "<h2>Config</h2><p>";
  response += "<form action=\"" + String(WEB_CONFIG_URL) + "\" method=\"post\">";
  response += "<h2>Connection Type</h2>";
  response += "<select name=\"connection_type\" />";
  response += "<option value=\"1\">Wifi</option>";
  response += "<option value=\"2\">Bluetooth</option>";
  response += "</select><br/>";
  response += "<input type=\"submit\" value=\"Save\"/>";
  response += "</form>";
  response += "</p></div>";
  //------------------------------------  Wifi  ------------------------------------
  response += "<div id=\"content-2\">";
  response += "<h2>Wifi</h2><p>";
  response += "SSID: " + config.getString("ssid", "") + "<br/>";
  response += "IP: " + WiFi.localIP().toString() + "<br/>";

  response += "<form action=\"" + String(WEB_CONFIG_URL) + "\" method=\"post\">";
  response += "<h2>Join Wifi network</h2>";
  response += "<select name=\"ssid\" />";

  int n = WiFi.scanNetworks(false, false, false, 100);
  for (int i = 0; i < n; ++i)
  {
    String selected = "";
    if(config.getString("ssid", "") == WiFi.SSID(i)) { selected = "selected"; }
    response += "<option value=\"" + WiFi.SSID(i) + "\" " + selected + ">" + WiFi.SSID(i) + "</option>";
  }
  response += "</select><br/>";

  //response += "SSID: <input type=\"text\" name=\"ssid\" value=\"" + config.getString("ssid", "") + "\"/><br/>";
  response += "Password: <input type=\"password\" name=\"wpa-psk\" value=\"\"/><br/>";
  response += "<h2>AP Mode</h2>";
  response += "When the configured SSID is not available, " + String(PRODUCT_NAME) + " will start an access point with these details.<br/>";
  response += "SSID: <input type=\"text\" name=\"ap-ssid\" value=\"" + config.getString("ap-ssid", "") + "\"/><br/>";
  response += "Password: <input type=\"password\" name=\"ap-psk\" value=\"\"/><br/>";
  response += "<input type=\"submit\" value=\"Save\"/>";
  response += "</form>";
  response += "</p></div>";
  //------------------------------------ Updates ------------------------------------
  response += "<div id=\"content-3\">";
  response += "<h2>Updates</h2><p>";
  response += "<h2>Upload Dashboard</h2><br/>";
  response += "<form action=\"" + String(UPDATE_DATA_UPLOAD_URL) + "\" method=\"post\" enctype=\"multipart/form-data\">";
  response += "File: <input type=\"file\" name=\"update\" id=\"dataFile\" /><br/>";
  //response += "<input type=\"submit\" accept=\".bin\" value=\"Upload\" onClick=\"uploadFile('dataFile', '" + String(UPDATE_DATA_UPLOAD_URL) + "');return false;\"/><br/>";
  response += "<input type=\"submit\" accept=\".bin\" value=\"Upload\"/>";
  //response += "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>";
  //response += "<p id=\"loaded_n_total\"></p>";
  response += "</form>";
  response += "<br/><br/>";
  response += "<h2>Upload Firmware</h2><br/>";
  response += "<form action=\"" + String(UPDATE_FW_UPLOAD_URL) + "\" method=\"post\" enctype=\"multipart/form-data\">";
  response += "File: <input type=\"file\" name=\"update\"/><br/>";
  response += "<input type=\"submit\" accept=\".bin\" value=\"Upload\"/>";
  response += "</form>";
  response += "</p></div>";

  response +="</div></div></body>";

  return response;
}

String webConfigPOSTRequest(AsyncWebServerRequest *request)
{
  bool wifiChanged = false;
  //Respond to any parameters that were sent through
  if (request->hasParam("ssid", true)) 
  {
    Serial.println("Changing Wifi network. New ssid: " + request->getParam("ssid", true)->value());
    config.putString("ssid", request->getParam("ssid", true)->value());
  }
  if (request->hasParam("wpa-psk", true)) 
  {
    Serial.println("New wifi password received");
    config.putString("wpa-psk", request->getParam("wpa-psk", true)->value());
  }
  if (request->hasParam("ap-ssid", true))
  {
    Serial.println("Changing AP SSID. New ssid: " + request->getParam("ap-ssid", true)->value());
    config.putString("ap-ssid", request->getParam("ap-ssid", true)->value());
  }
  if (request->hasParam("connection_type", true))
  {
    Serial.println("Changing Connection Type: " + request->getParam("connection_type", true)->value());
    //config.putUChar("connection_type", atoi(request->getParam("connection_type", true)->value()[0]) );
  }

  if(wifiChanged)
  {
    //Delayed reboot? Need to return the POST request and then reboot or restart network
  }

  return String("Config Saved");
}