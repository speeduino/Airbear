#include "globals.h"
#include "serialParser.h"
//#include <Arduino_JSON.h>
#include <ArduinoJson.h>

uint16_t tempRPM = 0;
uint8_t serialECURequestQueueSize = 0;

//#define DEBUG_SERIAL_PARSER

void requestSerialData() 
{
  #ifdef DEBUG_SERIAL_PARSER
    Serial.println("Requesting Serial Data");
  #endif
  Serial_ECU.write('A');
  serialECURequestQueueSize++;
}

void parseFixedSerialData() 
{
  
  #ifdef DEBUG_SERIAL_PARSER
    Serial.print("ECU Serial Data received. Received: ");
    Serial.print(Serial_ECU.available());
    Serial.println(" bytes");
  #endif
  

  if( (Serial_ECU.available() >= 75) && (Serial_ECU.read() == 'A') )
  {
    serialECURequestQueueSize--;
    //Serial.println("Valid Data found");
    //readings_JSON = undefined;

    readings_JSON["secl"] = Serial_ECU.read();

    uint8_t status1 = Serial_ECU.read();
    readings_JSON["inj1_status"] = BIT_CHECK(status1, 0);
    readings_JSON["inj2_status"] = BIT_CHECK(status1, 1);
    readings_JSON["inj3_status"] = BIT_CHECK(status1, 2);
    readings_JSON["inj4_status"] = BIT_CHECK(status1, 3);
    readings_JSON["dfco_active"] = BIT_CHECK(status1, 4);
    readings_JSON["boost_cut_fuel"] = BIT_CHECK(status1, 5);
    
    uint8_t engine = Serial_ECU.read();
    readings_JSON["running"] = BIT_CHECK(engine, 0);

    readings_JSON["dwell"] = Serial_ECU.read() / 10.0;
    readings_JSON["MAP"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    readings_JSON["IAT"] = Serial_ECU.read() - CALIBRATION_TEMPERATURE_OFFSET;
    readings_JSON["CLT"] = Serial_ECU.read() - CALIBRATION_TEMPERATURE_OFFSET;
    readings_JSON["correction_voltage"] = Serial_ECU.read();
    readings_JSON["Battery_Voltage"] = Serial_ECU.read() / 10.0;
    readings_JSON["AFR1"] = Serial_ECU.read() / 10.0;
    readings_JSON["correction_o2"] = Serial_ECU.read();
    readings_JSON["correction_iat"] = Serial_ECU.read();  
    readings_JSON["correction_wue"] = Serial_ECU.read();
    readings_JSON["rpm"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    #ifdef FAKE_RPM
      tempRPM += 16;
      readings_JSON["rpm"] = tempRPM;
      if(tempRPM >= 9000) tempRPM = 0;
    #endif

    readings_JSON["correction_ae"] = Serial_ECU.read();
    readings_JSON["correction_total"] = Serial_ECU.read();
    readings_JSON["VE"] = Serial_ECU.read();
    readings_JSON["afr_target"] = Serial_ECU.read() / 10.0;
    readings_JSON["PW1"] = (Serial_ECU.read() | (Serial_ECU.read() << 8)) / 10;0;
    readings_JSON["tps_DOT"] = Serial_ECU.read() * 10;
    readings_JSON["advance"] = Serial_ECU.read();
    readings_JSON["TPS"] = Serial_ECU.read();

    readings_JSON["loops_per_second"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    readings_JSON["free_ram"] = Serial_ECU.read() | (Serial_ECU.read() << 8);

    readings_JSON["boost_target"] = Serial_ECU.read() * 2;
    readings_JSON["boost_duty"] = Serial_ECU.read();
    readings_JSON["spark_bits"] = Serial_ECU.read();
    readings_JSON["rpmDOT"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    readings_JSON["ethanol%"] = Serial_ECU.read();
    readings_JSON["flex_correction"] = Serial_ECU.read();
    readings_JSON["flex_ign_correction"] = Serial_ECU.read();
    readings_JSON["idle_load"] = Serial_ECU.read();
    readings_JSON["test_outputs"] = Serial_ECU.read();
    readings_JSON["AFR2"] = Serial_ECU.read();
    readings_JSON["baro"] = Serial_ECU.read();
    readings_JSON["AFR2"] = Serial_ECU.read();

    //CAN Status variables 1 to 16
    for(uint8_t i = 1; i <= 16; i++)
    {
      readings_JSON["CAN_Status_" + String(i)] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    }

    while(Serial_ECU.available())
    {
      Serial_ECU.read();
    }
  }
  else
  {
    return;
  }
}

void initSerialData()
{
    readings_JSON["secl"] = 0;

    uint8_t status1 = 0;
    readings_JSON["inj1_status"] = BIT_CHECK(status1, 0);
    readings_JSON["inj2_status"] = BIT_CHECK(status1, 1);
    readings_JSON["inj3_status"] = BIT_CHECK(status1, 2);
    readings_JSON["inj4_status"] = BIT_CHECK(status1, 3);
    readings_JSON["dfco_active"] = BIT_CHECK(status1, 4);
    readings_JSON["boost_cut_fuel"] = BIT_CHECK(status1, 5);
    
    uint8_t engine =0;
    readings_JSON["running"] = BIT_CHECK(engine, 0);

    readings_JSON["dwell"] = 0;
    readings_JSON["MAP"] = 0;
    readings_JSON["IAT"] = 0;
    readings_JSON["CLT"] = 0;
    readings_JSON["TPS"] = 0;
    readings_JSON["correction_voltage"] = 0;
    readings_JSON["Battery_Voltage"] = 0;
    readings_JSON["AFR1"] = 0;
    readings_JSON["PW1"] = 0;
    readings_JSON["correction_o2"] = 0;
    readings_JSON["correction_iat"] = 0;
    readings_JSON["correction_clt"] =0;
    readings_JSON["correction_wue"] = 0;
  
    readings_JSON["rpm"] = 0;
}

