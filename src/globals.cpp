#include "globals.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an SSE object
AsyncEventSource events("/events");

// Json Variable for current data packet
//JSONVar readings_JSON;
JsonDocument readings_JSON;

volatile byte TIMER_mask;
volatile byte LOOP_TIMER;

bool deviceConnected = false;
volatile uint8_t requestCount = 0;