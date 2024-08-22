#ifndef BLE_UART_H
#define BLE_UART_H
#include <NimBLEDevice.h>

#define BLE_LONG_STRING         (false) //Likely only works on iOS?

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"        // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class Callbacks: public BLECharacteristicCallbacks 
{
  private:
    void onWrite(BLECharacteristic *pCharacteristic);
};

class ServerCallbacks: public BLEServerCallbacks 
{
  private:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

void initBLE(void);
void SendMessageBLE(std::string);

#endif