#include "globals.h"
#include "config.h"

Preferences config;

void initConfig()
{
  config.begin("air-bear", false);

  if(config.getUChar("connection_type", 0) == 0) { config.putUChar("connection_type", CONNECTION_TYPE_DASH); } //Default to WIFI mode

  Serial.println("Config Initialised");
}