#ifndef WEB_CONFIG_H
#define WEB_CONFIG_H

#include "globals.h"

#define WEB_CONFIG_URL "/config"

String webConfigRequest(AsyncWebServerRequest *request);
String webConfigPOSTRequest(AsyncWebServerRequest *request);

#endif