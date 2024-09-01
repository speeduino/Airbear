#include "tcp-uart.h"
#include <AsyncTCP.h>

AsyncServer *TS_TCP_Server;
uint32_t TCPrequestsReceived = 0;
uint8_t numTCPClients = 0;

void handleData(void *arg, AsyncClient *client, void *data, size_t len)
{
  Serial.printf("%d bytes received from client %s: ", len, client->remoteIP().toString().c_str());
  Serial.write((uint8_t *)data, len);
  Serial.println("");
  TCPrequestsReceived++;

  for(int i = 0; i < len; i++)
  {
    Serial_ECU.write(((uint8_t *)data)[i]);
  }

  //Wait for response from ECU
  while(Serial_ECU.available() < 1) { }
  //Serial.print("Response received from ECU: ");
  //Serial.println((char)Serial_ECU.peek());

  //Read byte and see if it is the 'F' or 'Q' character for the first connection
  if( ((uint8_t *)data)[0] == 'F')
  {
    //Serial.println("Received an F command");

    //Wait for 3 byte response
    while(Serial_ECU.available() < 3) { }
    char c[3];
    Serial_ECU.readBytes(c, 3);
    //Serial.println(c);

    if(client->space() >= 3 )
    {
      //Serial.println("Sent reply");
      client->add(c, 3);
      client->send();
    }
  }
  else if( (((char *)data)[0] == 'Q') || (((char *)data)[0] == 'S'))
  {
    //Serial.println("Received a Q or S command");

    //Wait for 3 byte response
    delay(40);
    //char c[3];
    //Serial_ECU.readBytes(c, 3);
    //Serial.println(c);

    if(client->space() >= Serial_ECU.available() )
    {
      //Serial.println("Sent reply");
      while(Serial_ECU.available())
      {
        char c = Serial_ECU.read();
        client->add(&c, 1);
      }
      //client->add(c, 3);
      client->send();
    }
  }
  else
  {
    //Serial.println("Received a different command");

    //If the first character wasn't 'F' then the client is already connected and the new protocol should be used
    //Wait for the 2nd byte of the package
    while(Serial_ECU.available() < 2) { }

    //Read size of response from ECU
    uint8_t sizeHighByte = Serial_ECU.read();
    uint8_t sizeLowByte = Serial_ECU.read();
    uint16_t serial_len = word(sizeHighByte, sizeLowByte);
    serial_len += 4; //Add 4 bytes for the CRC
    Serial.printf("Expecting %d bytes from ECU \n", serial_len);

    //Wait for all data to come through.
    //Total size is the data packet (serial_len) + 4 bytes for the CRC
    //TODO: Implement a timeout handler
    uint16_t counter = 0;
    while(Serial_ECU.available() < (serial_len-1)) 
    { 
      Serial.printf("%d bytes received from ECU \n", Serial_ECU.available()); 
      delay(1);
      counter++;
      if(counter > 2000) { break; }
    }
    Serial.printf("%d bytes ready from ECU \n", Serial_ECU.available()); 
    
    char c[serial_len];
    Serial_ECU.readBytes(c, serial_len);
    
    if(client->space() >= serial_len )
    {
      Serial.print("Sent data: ");
      Serial.println(serial_len);
      client->add((char*)&sizeHighByte, 1);
      client->add((char*)&sizeLowByte, 1);
      client->add(c, serial_len);
      client->send();
    }
  }

  //Clear buffer in case anything is left
  while(Serial_ECU.available() > 0) { Serial_ECU.read(); }

}

void handleNewClient(void *arg, AsyncClient *client)
{
	Serial.printf("New client has been connected to TCP socket, ip: %s \n", client->remoteIP().toString().c_str());
	// register events
  numTCPClients++;
	client->onData(&handleData, NULL);
  //TS_TCP_Client->onData(&handleData, NULL);
	//client->onError(&handleError, NULL);
	//client->onDisconnect(&handleDisconnect, NULL);
	//client->onTimeout(&handleTimeOut, NULL);
}

void initTCP()
{
  AsyncServer *TS_TCP_Server = new AsyncServer(2000);
  TS_TCP_Server->onClient(&handleNewClient, TS_TCP_Server);
  TS_TCP_Server->begin();
}