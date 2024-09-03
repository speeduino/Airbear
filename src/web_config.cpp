#include "web_config.h"
#include "updater.h"
#include "config.h"

String webConfigRequest(AsyncWebServerRequest *request)
{
  String response = "";
  response += "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Speeduino Web Config</title>";
  //response += "<script>function _(e){return document.getElementById(e)}function uploadFile(e,n){var a=_(e).files[0],o=new FormData;o.append(e,a);var t=new XMLHttpRequest;t.upload.addEventListener(\"progress\",progressHandler,!1),t.open(\"POST\",n),t.send(o)}function progressHandler(e){_(\"loaded_n_total\").innerHTML=\"Uploaded \"+e.loaded+\" bytes of \"+e.total;var n=e.loaded/e.total*100;_(\"progressBar\").value=Math.round(n)}</script>";
  
  response += "<style>";
  //This is a minified version of the data/css/config.css file.
  response += "body{font-family:'Arial';display:block;align-items:center;justify-content:center;height:100%;padding:0;margin:0;box-sizing:border-box;color:#fff;background-color:#202020;background-image:linear-gradient(to right,rgb(0 0 0),#fff0 30%,#fff0 70%,rgb(0 0 0)),linear-gradient(45deg,black 25%,transparent 25%,transparent 75%,black 75%,black),linear-gradient(45deg,black 25%,transparent 25%,transparent 75%,black 75%,black),linear-gradient(to bottom,#080808,#202020);background-size:100% 100%,10px 10px,10px 10px,10px 5px;background-position:0 0,0 0,5px 5px,0 0}html{box-sizing:border-box;height:100%}small{opacity:.4}h1{color:#fff;text-shadow:2px 2px 2px rgb(0 0 0 / .6);text-align:center;margin-bottom:0;font-size:1.5em;line-height:1em;font-weight:700;margin-top:.27em}.container{margin-bottom:20px;margin-top:30px;min-width:620px;max-width:700px;margin-left:auto;margin-right:auto;width:50%}p{margin:0 0 20px;line-height:1.5;font-weight:300}.content{background:#fff;color:#373737;backface-visibility:hidden;overflow:hidden}.content>div{display:none;padding:20px 25px 5px}input.tabs{display:none}label{display:inline-block;padding:15px 25px;font-weight:600;text-align:center;border-bottom:1px solid #fff0;transition:all .3s ease-out 0.1s}label:hover{color:#fff;cursor:pointer}input:checked+label{background:#fff;color:#fd7034}#tab-1:checked~.content #content-1,#tab-2:checked~.content #content-2,#tab-3:checked~.content #content-3,#tab-4:checked~.content #content-4{display:block;animation-name:inUp;animation-timing-function:ease-in-out;animation-duration:.6s}@media screen and (max-width:400px){label{padding:15px 10px}}@keyframes inUp{0%{opacity:0;transform:translateY(100px)}90%{transform:translateY(-10px)}100%{opacity:1;transform:translateY(0)}}";
  response += "</style>";
  //response += "<link rel=\"stylesheet\" href=\"/css/config.css\" />"; // <--- Alternative

  response += "</head>";
  response += "<body>";
  response += "<div class=\"container\">";
  response += "<center><img width='70%' src=\"";
  response += "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' preserveAspectRatio='xMidYMid meet' viewBox='0 0 18830 5580'%3E%3Cg fill='%23FFF'%3E%3Cpath d='M13040 170c-130 120-180 170-180 190s20 40 120 140c110 130 130 150 90 180-20 30-350 20-420 0-50-30-80-10-160 70l-60 60-5210 10c-5720 10-5270 0-5460 60-200 60-310 160-400 330-50 100-50 100-670 1740C-40 4860-10 4770 0 4880c20 130 130 210 380 240l670 20h600v-110c10-100 10-100 40-130l70-40c80-10 80-10 80-50v-30h-190c-670 0-880-70-870-310 0-90 10-120 160-530l800-2100c120-330 200-440 380-530 140-80 270-100 530-120l4970-10h4800l60 60 90 80c40 10 80 10 130-20l170-10h130l130 70c150 90 150 90 200 160l40 60h70l120-20 160-10c90 0 100 0 130-30l20-30h390l390-10c20 0 20-20 20-50s10-40 30-60 40-20 260-20h240l40-50c40-40 50-40 110-40 50 0 60 0 100 30l70 40 60-50c30-40 50-50 80-50 20 0 30-10 50-40l20-30h70c90 0 90 0 90-80l10-50 110 40c90 30 150 40 150 20V840l-40 10-140 30-90 40v-60l-10-70-80-10c-50-10-70-20-90-40l-50-20c-20 0-40-20-70-60-60-60-90-70-140-20-30 30-40 30-120 30-90 0-90 0-130-30l-40-30h-460l-80-100-90-90h-180c-180 0-180 0-220 20-30 30-40 30-140 30s-120 0-130-20-40-20-150-20c-150 0-150 0-150-110v-80l-90-80c-80-90-90-90-120-80-30 0-40 0-80-40l-50-40-180 170'/%3E%3Cpath d='m16720 870-60 170-50 130 390 10c690 0 870 20 980 130 80 70 100 140 70 280a58690 58690 0 01-1130 2900c-90 110-270 210-460 250-180 30-150 30-5470 40a167480 167480 0 00-5580 20h-70v-40c0-30-10-40-30-40s-50-10-60-30c-40-30-80-30-120 0l-30 20-30-30c-40-40-50-40-80 0s-60 40-90 0c-30-30-40-30-70 0s-50 40-70 10c-30-30-30-30-90 0l-40 20-30-30c-20-10-40-20-50-10h-150c-20-10-110-20-230-20l-200-10-10-30c-10-40-40-80-100-100-30-20-40-30-40-60l-20-70-20-30h-100l-110 10-20 90-20 60-70 10c-60 0-70 0-90-30l-20-20h-390l-10 30-20 30-30-30c-10-20-40-30-50-30-30 0-80 50-80 80 0 40-40 80-60 60h-70l-30 10c-10 10-20 10-30-10h-30c-10 10-10 20-30 0h-40c-10 20-20 20-30 0s-20-10-30 0-20 20-30 0h-100c-10 20-20 20-30 0h-130l-30 10-10 10-20-10c0-20-30-10-40 10l-40 20c-60 0-70 20-70 160v110l-40 10c-70 20-70 60-10 80l50 10v150c-20 40-110 30-120-10l-10-120c0-70-10-90-20-90-40-10-50 20-50 130l10 130c10 20 50 40 130 50l80 20 60 10c30 0 40 0 60 20 10 30 40 40 60 20h620c20 0 30 10 30 20 10 50 10 70 30 90 30 30 60 20 100-10l30-30 20 30 10 30h400l20-30c10-20 20-20 80-20h70l20 70 30 80 90 20c90 0 90 0 110-30 20-10 30-40 40-70 10-50 10-50 50-70 60-10 90-50 90-90l10-40 180-10 220-20 100-10c60 10 80 10 120-10l50-20 30 20 40 30 40-30c50-40 60-40 90 0s40 40 70 0 50-40 90 0l30 30 30-30 50-30 30 30 30 30 70-30 80-40c10-10 20-20 10-30v-30h80l100-20c30-10 30-10 130 10l110 20h5490c5900-10 5580 0 5790-60 230-50 360-160 460-370 20-40 120-280 210-540l350-900 310-810c470-1260 470-1260 470-1380 0-60-10-70-40-110-60-80-170-120-380-140l-890-10h-780zM2550 1860c-300 60-560 280-630 540-20 60-10 190 10 260 20 80 40 140 140 320s120 230 140 320c40 260-180 490-440 480-100-10-170-40-260-100l-70-60-60 60-140 170c10 20 110 100 160 120 160 80 420 100 640 30 150-40 280-120 390-240 130-140 190-290 190-460 0-120-20-190-90-340-190-340-240-460-200-580 20-110 120-210 240-250l120-10c80 0 150 20 220 70l30 20 30-30 180-170c0-20-140-100-210-130-100-30-260-40-390-20'/%3E%3Cpath d='M16460 1870c-300 50-570 210-810 460-250 260-420 570-480 860-20 130-20 310 10 410 30 120 70 200 150 270 130 130 250 170 480 170 140 0 220-10 340-50 390-120 800-490 980-900 140-300 190-600 140-810-60-200-210-350-410-410-90-20-290-20-400 0zm170 290c240 50 300 280 190 660-110 420-380 770-670 880-80 40-210 50-300 40-250-40-340-290-240-680 80-290 240-560 450-730 190-140 390-200 570-170zM3680 1980l-150 350-300 710-240 550-180 420h190l190-10 60-140 180-400 170-400 390-900c20-20 200-10 280 10 90 30 140 120 130 230-30 300-270 520-570 540h-40l-50 110-60 130a1140 1140 0 00 980-440c70-100 90-160 120-250 60-200 30-370-90-470-50-50-100-80-200-100-70-20-110-20-420-30h-350zm1540 420-320 740-210 480a22680 22680 0 01-170 390h1140l50-120 50-130-370-10-370-10 30-80 110-240 120-280 50-110h310l310-10 60-120 50-120-300-10-310-10 20-70 140-310 100-230h640l10-40 60-130 40-90H5430zm1640-260-330 760-80 170a234380 234380 0 01-400 940h1140l50-120 50-130-370-10-370-10 150-360 160-350h310l310-10 60-120 50-130a6070 6070 0 00-610-10v-20l130-290 120-290 330-10h320l10-40 60-130 40-90H6960zm1570-110-360 850-130 300-230 530-130 290 330 10c460 0 610-20 820-90 570-180 990-660 1110-1220 30-130 30-300 10-390-50-200-200-330-400-390-120-20-240-30-600-30h-360zm750 140a370 370 0 01 270 330c10 180-70 460-200 660l-250 280c-180 150-450 280-640 300l-280 10 180-430 340-790 170-380h170l240 20zm1340-90-210 490c-290 660-360 850-380 990-20 150 20 260 110 350 100 100 220 130 420 130 210 0 380-40 550-140 150-90 300-230 400-380a3680 3680 0 00 320-650l150-350 180-420 90-210h-340l-20 50-250 570c-260 620-340 780-430 910-130 200-260 290-440 320-150 20-260-20-310-120-20-40-20-50-20-130 10-120 40-190 360-930l170-400 80-190 30-70-190-10h-190zm2060 60-220 490a90310 90310 0 01-590 1370l190 10 190-10 60-120a50530 50530 0 01 630-1460l130-310 90-210-190-10h-190zm1030-190-200 460-690 1600h330l40-110a4954010 4954010 0 01 620-1430l40 150c50 230 190 740 250 940l80 260 50 190h140l140-10 90-200 180-410 250-580 210-480 110-260 70-170-160-10h-160l-160 360-510 1190-30-150c-60-260-230-850-350-1270l-40-130h-270'/%3E%3C/g%3E%3C/svg%3E%0A";
  response += "\"></center><br />";
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
  response += "<option value=\"1\" " + (config.getUChar("connection_type") == CONNECTION_TYPE_WIFI ? String("selected") : String("")) + ">Web Dash</option>";
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
    char c = request->getParam("connection_type", true)->value()[0];
    config.putUChar("connection_type", atoi(&c) );
  }

  if(wifiChanged)
  {
    //Delayed reboot? Need to return the POST request and then reboot or restart network
  }

  return String("Config Saved");
}