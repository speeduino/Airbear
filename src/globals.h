#ifndef GLOBALS_H
#define GLOBALS_H

//#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define PRODUCT_NAME             "AirBear"
#define FIRMWARE_VERSION         "0.0.9"

//#define FAKE_RPM

#define BIT_SET(a,b) ((a) |= (1U<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1U<<(b)))
#define BIT_CHECK(var,pos) !!((var) & (1U<<(pos)))
#define BIT_TOGGLE(var,pos) ((var)^= 1UL << (pos))

#define BIT_TIMER_1HZ             0U
#define BIT_TIMER_4HZ             1U
#define BIT_TIMER_10HZ            2U
#define BIT_TIMER_15HZ            3U
#define BIT_TIMER_30HZ            4U
#define BIT_TIMER_200HZ           6U
#define BIT_TIMER_1KHZ            7U

#define CONNECTION_TYPE_NONE        0
#define CONNECTION_TYPE_DASH        1
#define CONNECTION_TYPE_BLE         2
#define CONNECTION_TYPE_TUNERSTUDIO 3

#define BLE_ID                    "Speeduino"

#if ARDUINO_USB_CDC_ON_BOOT
  #define Serial_ECU              Serial0
#else
  #define Serial_ECU              Serial1
#endif

#ifndef U_FLASH
  #define U_FLASH  0
  #define U_SPIFFS 100
  #define U_AUTH   200
#endif

extern volatile byte TIMER_mask;
extern volatile byte LOOP_TIMER;

// Create AsyncWebServer object on port 80
extern AsyncWebServer server;

// Create a SSE object
extern AsyncEventSource events;

// Json Variable for current data packet
//extern JSONVar readings_JSON;
extern JsonDocument readings_JSON;

extern bool deviceConnected;

#endif