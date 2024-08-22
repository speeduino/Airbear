
#ifndef SSE_H
#define SSE_H

extern uint32_t notificationsSent;

void initSSE();
void notifyClients();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void onConnect(AsyncEventSourceClient *client);
void sendPing();
void sendNoDataMessage();

#endif