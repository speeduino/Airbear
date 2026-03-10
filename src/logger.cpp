#include "logger.h"
#include "config.h"
#include "sse.h"

void debugMsg(String msg, uint8_t pri)
{
  if(pri >= config.getUChar("debug_level"))
  {
    if(config.getBool("debug_serial"))
    {
      Serial.println(msg);
    }
    if(config.getBool("debug_web"))
    {
      sendSSEDebugMessage(msg);
      Serial.println(msg);
    }
  }
}