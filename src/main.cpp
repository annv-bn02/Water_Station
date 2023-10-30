/*
  ModbusRTU ESP8266/ESP32
  Simple slave example

  (c)2019 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266

  modified 13 May 2020
  by brainelectronics

  This code is licensed under the BSD New License. See LICENSE.txt for more info.
*/

#include "all_header.h"
#define TEST_MODBUS 0
#define TEST_LED_BUT 1
#define TEST_DS1307 0
#define TEST_ADC 0
#define TEST_MAX6369 1
#define TEST_SIM800L 1

uint32_t counttt = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  OTA_Setup();
#if TEST_MODBUS
  MB_Slave_Setup();
#endif
#if TEST_LED_BUT
  TLB_Config();
#endif
#if TEST_DS1307
  MD_Config();
#endif
#if TEST_ADC
  TA_Config();
#endif
#if TEST_MAX6369
  TM_Config();
#endif
#if TEST_SIM800L
  delay(5000);
  TS_Setup();
#endif
  
}

void loop() {
#if TEST_MODBUS
  MB_Slave_Run();
#endif
#if TEST_LED_BUT
  // TLB_Blink();
  TLB_Button();
#endif
#if TEST_DS1307
  MD_Run();
#endif
#if TEST_ADC
  TA_Run();
#endif
#if TEST_MAX6369
  TM_Run();
#endif
#if TEST_SIM800L
  TS_Read_SMS();
#endif
  // counttt++;
  // if(counttt == 10000)
  // {
  //   TS_Send_SMS();
  //   counttt = 0;
  // }
  OTA_update();
  delay(1);
}

