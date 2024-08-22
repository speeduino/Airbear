#include "ble-uart.h"
#include "globals.h"
#include <NimBLEDevice.h>

//Much of this file based on: https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/examples/BLE_uart/BLE_uart.ino

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool oldDeviceConnected = false;
std::string ReceivedMessageBLE;


void initBLE(void) 
{
  deviceConnected = false;
  BLEDevice::init(BLE_ID);                                                                              // Create the BLE Device
  pServer = BLEDevice::createServer();                                                                  // Create the BLE Server
  pServer->setCallbacks(new ServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);                                          // Create the BLE Service
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);  // Create a BLE Characteristic                      
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
     
  pRxCharacteristic->setCallbacks(new Callbacks());
  pService->start(); 
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID); 
  pServer->start();                                                                            // Start the server  
  pServer->getAdvertising()->start();                                                          // Start advertising

  Serial.println(BLEDevice::getAddress());
}

void ServerCallbacks::onConnect(BLEServer* pServer) 
{
  deviceConnected = true;
};

void ServerCallbacks::onDisconnect(BLEServer* pServer) 
{
  deviceConnected = false;
}

void Callbacks::onWrite(BLECharacteristic *pCharacteristic) 
{
  std::string rxValue = pCharacteristic->getValue();

  if (rxValue.length() > 0) {
    Serial.println("*********");
    Serial.print("Received Value: ");
    for (int i = 0; i < rxValue.length(); i++)
    {
      Serial.print(rxValue[i]);
      Serial.print(" Length: ");
      Serial.print(rxValue.length());

      Serial_ECU.write(rxValue[i]);
    } 

    Serial.println();
    Serial.println("*********");
  }
}

void SendMessageBLE(std::string Message)
{
  if(deviceConnected) 
  {
    if (BLE_LONG_STRING)                                                                 // If Fast transmission is possible
    {
      pTxCharacteristic->setValue(Message); 
      pTxCharacteristic->notify();
      delay(10);                                                                              // Bluetooth stack will go into congestion, if too many packets are sent
    } 
    else                                                                                      // Packets of max 20 bytes
    {   
      int parts = (Message.length()/20) + 1;
      for(int n=0;n<parts;n++)
      {   
        pTxCharacteristic->setValue(Message.substr(n*20, 20)); 
        pTxCharacteristic->notify();
        delay(10);                                                                           // Bluetooth stack will go into congestion, if too many packets are sent. Can possibly fine tune this down, needs testing
      }
    }
  } 
}