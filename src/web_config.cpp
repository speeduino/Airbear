#include "web_config.h"
#include "updater.h"
#include "config.h"
#include "logger.h"
#include "WiFi.h"
#include "static/static_html.h"
#include "static/static_js.h"
#include "static/static_css.h"

String webConfigRequest(AsyncWebServerRequest *request)
{
  String response = "";
  response += staticHTML_head();
  response += "<title>Speeduino Web Config</title>";
  response += staticJS_updates();
  
  //This is a minified version of the static/static_css.css file.
  response += staticCSS_config();
  //response += "<link rel=\"stylesheet\" href=\"/css/config.css\" />"; // <--- Alternative

  response += "</head>";
  response += "<body onLoad=\"getLatestGithubRelease('" + String(FIRMWARE_VERSION) + "')\">";
  response += "<div class=\"container\">";
  response += staticHTML_logo();
  response += "<input id=\"tab-1\" type=\"radio\" name=\"tabs\" class=\"tabs\" checked>";
  response += "<label for=\"tab-1\">Config</label>";
  response += "<input id=\"tab-2\" type=\"radio\" name=\"tabs\" class=\"tabs\">";
  response += "<label for=\"tab-2\">Wifi</label>";
  response += "<input id=\"tab-3\" type=\"radio\" name=\"tabs\" class=\"tabs\">";
  response += "<label for=\"tab-3\">Update</label>";
  response += "<input id=\"tab-4\" type=\"radio\" name=\"tabs\" class=\"tabs\">";
  response += "<label for=\"tab-4\">Debug</label>";
  
  response += "<div class=\"content\">";
  //------------------------------------ Config ------------------------------------
  response += "<div id=\"content-1\">";
  response += "<h2>Config</h2><p>";
  response += "<form action=\"" + String(WEB_CONFIG_URL) + "\" method=\"post\">";
  response += "<h2>Connection Type</h2>";
  response += "<select name=\"connection_type\" />";
  response += "<option value=\"1\" " + (config.getUChar("connection_type") == CONNECTION_TYPE_DASH ? String("selected") : String("")) + ">Web Dash</option>";
  response += "<option value=\"2\" " + (config.getUChar("connection_type") == CONNECTION_TYPE_BLE  ? String("selected") : String("")) + ">Bluetooth</option>";
  response += "<option value=\"3\" " + (config.getUChar("connection_type") == CONNECTION_TYPE_TUNERSTUDIO ? String("selected") : String("")) + ">TunerStudio (TCP)</option>";
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
  response += "<select id=\"ssid\" name=\"ssid\" />";
  response += "</select>";
  response += "<button type=\"button\" onclick=\"scanWifi()\">Scan</button><br/>";

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
  response += "<h2>Auto Update</h2>";
  response += "Current firmware version: " + String(FIRMWARE_VERSION) + "<br/>";
  response += "Latest firmware version: <span id='latest_release_txt'></span><br/>";
  response += "<form action=\"" + String(UPDATE_REMOTE_URL) + "\" method=\"post\">";
  response += "<input type=\"hidden\" id=\"newFW_url\" name=\"newFW_url\" />";
  response += "<input type=\"hidden\" id=\"newData_url\" name=\"newData_url\"/>";
  response += "<input type=\"checkbox\" id=\"updateData\" onClick=\"toggleData()\" checked />Update Dashboard Data<br/>";
  response += "<input type=\"submit\" value=\"Update\" id=\"update_btn\" disabled/><br/>";
  response += "</form><hr/>";
  response += "<h2>Manual Update</h2>";
  response += "<h3>Upload Filesystem</h3>";
  response += "<form action=\"" + String(UPDATE_DATA_UPLOAD_URL) + "\" method=\"post\" enctype=\"multipart/form-data\">";
  response += "File: <input type=\"file\" name=\"update\" id=\"dataFile\" /><br/>";
  //response += "<input type=\"submit\" accept=\".bin\" value=\"Upload\" onClick=\"uploadFile('dataFile', '" + String(UPDATE_DATA_UPLOAD_URL) + "');return false;\"/><br/>";
  response += "<input type=\"submit\" accept=\".bin\" value=\"Upload\"/>";
  //response += "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>";
  //response += "<p id=\"loaded_n_total\"></p>";
  response += "</form>";
  response += "<h3>Upload Firmware</h3>";
  response += "<form action=\"" + String(UPDATE_FW_UPLOAD_URL) + "\" method=\"post\" enctype=\"multipart/form-data\">";
  response += "File: <input type=\"file\" name=\"update\"/><br/>";
  response += "<input type=\"submit\" accept=\".bin\" value=\"Upload\"/>";
  response += "</form>";
  response += "</p></div>";
  //------------------------------------ Debug ------------------------------------
  response += "<div id=\"content-4\">";
  response += "<h2>Debug Options</h2><p>";
  response += "<form action=\"" + String(WEB_CONFIG_URL) + "\" method=\"post\">";
  response += "<select name=\"debug_level\" />";
  response += "<option value=\"0\" " + (config.getUChar("debug_level") == LOG_LEVEL_OFF ? String("selected") : String("")) + ">Off</option>";
  response += "<option value=\"1\" " + (config.getUChar("debug_level") == LOG_LEVEL_INFO ? String("selected") : String("")) + ">Info</option>";
  response += "<option value=\"2\" " + (config.getUChar("debug_level") == LOG_LEVEL_WARN ? String("selected") : String("")) + ">Warn</option>";
  response += "<option value=\"3\" " + (config.getUChar("debug_level") == LOG_LEVEL_ERROR ? String("selected") : String("")) + ">Error</option>";
  response += "<option value=\"4\" " + (config.getUChar("debug_level") == LOG_LEVEL_FATAL ? String("selected") : String("")) + ">Fatal</option>";
  response += "</select><br/>";
  response += "<input type=\"checkbox\" name=\"debug_serial\" "+ (config.getBool("debug_serial", false) ? String("checked") : String("")) + "/>Debug to Serial<br/>";
  response += "<input type=\"checkbox\" name=\"debug_web\" "+ (config.getBool("debug_web", false) ? String("checked") : String("")) + "/>Debug to Web Dash<br/>";
  response += "<input type=\"submit\" value=\"Save\"/></p>";
  response += "<h2>Debug Output</h2><p>";
  response += "<textarea id=\"debug_text\" rows=\"10\" cols=\"50\" readonly></textarea>";
  response += "</form>";
  response += "</p></div>";
  response +="</div></div></body>";
  return response;
}

String webConfigPOSTRequest(AsyncWebServerRequest *request)
{
  bool wifiChanged = false;
  String updateMessage = "";
  //Respond to any parameters that were sent through
  if (request->hasParam("ssid", true)) 
  {
    updateMessage = "Changing Wifi network. New ssid: " + request->getParam("ssid", true)->value();
    config.putString("ssid", request->getParam("ssid", true)->value());
  }
  if (request->hasParam("wpa-psk", true)) 
  {
    updateMessage = "New wifi password received";
    config.putString("wpa-psk", request->getParam("wpa-psk", true)->value());
  }
  if (request->hasParam("ap-ssid", true))
  {
    updateMessage = "Changing AP SSID. New ssid: " + request->getParam("ap-ssid", true)->value();
    config.putString("ap-ssid", request->getParam("ap-ssid", true)->value());
  }
  if (request->hasParam("connection_type", true))
  {
    updateMessage = "Changing Connection Type: " + request->getParam("connection_type", true)->value();
    char c = request->getParam("connection_type", true)->value()[0];
    config.putUChar("connection_type", atoi(&c) );
  }
  if (request->hasParam("debug_level", true))
  {
    updateMessage = "Changing Debug Level: " + request->getParam("debug_level", true)->value();
    char c = request->getParam("debug_level", true)->value()[0];
    config.putUChar("debug_level", atoi(&c) );
  }
  if (request->hasParam("debug_serial", true)) { config.putBool("debug_serial", true); }
  else { config.putBool("debug_serial", false); }
  if (request->hasParam("debug_web", true)) { config.putBool("debug_web", true); }
  else { config.putBool("debug_web", false); }
  Serial.println(updateMessage);

  //Create the updates page
  String updatePage = staticHTML_head();
  updatePage += staticCSS_config();
  updatePage += "<meta http-equiv=\"refresh\" content=\"7\"; url=\"/\" />";
  updatePage += "</head><body>";
  updatePage += "<div class=\"container\">";
  updatePage += staticHTML_logo();
  updatePage += "<input id=\"tab-1\" type=\"radio\" name=\"tabs\" class=\"tabs\" checked>";
  updatePage += "<label for=\"tab-1\">Config</label>";
  updatePage += "<div class=\"content\">";

  updatePage += "<div id=\"content-1\">";
  updatePage += "<center>";
  updatePage += "<h2>Config Saved</h2><br/>";
  updatePage += "Please wait 10 seconds for system to restart";
  updatePage += "</center></br></br></br></br>";
  updatePage += "<strong>Message: </strong>";
  updatePage += updateMessage;
  
  updatePage += "<br/></div></div></body></html>";

  return updatePage;
}

String scanWifi(AsyncWebServerRequest *request)
{
  JsonDocument json;
  int n = WiFi.scanNetworks(false, false, false, 100);
  for (int i = 0; i < n; ++i)
  {
    json["networks"][i]["ssid"] = WiFi.SSID(i);
  }
  String output;
  serializeJson(json, output);

  return output;
}
