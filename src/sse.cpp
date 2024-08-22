#include "globals.h"
#include "sse.h"

uint32_t notificationsSent = 0;

void initSSE() 
{
  //ws.onEvent(onEvent);
  events.onConnect(onConnect);
  server.addHandler(&events);
}

void notifyClients() 
{
  //if (WiFi.status() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) 
  if(false)
  {
    //See: https://github.com/me-no-dev/ESPAsyncWebServer/issues/1158#issuecomment-1557017468
  }
  else
  {
    events.send(JSON.stringify(readings_JSON).c_str(), "reading", millis(), 0);
    notificationsSent++;
  }
}

void sendPing()
{
  events.send("ping",NULL,millis());
  //events.send(String(loopCounter).c_str(),"ping",millis(), 0);
}

void sendNoDataMessage()
{
  //Alert the client that there is no data being received from the ECU
  events.send("nodata", "nodata", millis(), 0);
}

void onConnect(AsyncEventSourceClient *client) 
{
  if(client->lastId())
  {
    Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
  }
  // send event with message "hello!", id current millis
  // and set reconnect delay to 1 second
  Serial.print("Client connected: ");
  Serial.println(client->lastId());
  client->send("hello!", NULL, millis(), 10000);
}