#include "all_header.h"
#define TEST_MODBUS 0
#define TEST_BUTTON 1
#define TEST_DS1307 1
#define TEST_ADC 1
#define TEST_MAX6369 1
#define TEST_SIM800L 1
uint32_t count_test = 0;
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
#if DEBUG_WEB
  WD_Setup();
#endif
#if TEST_MODBUS
  MB_Slave_Setup();
#endif
#if TEST_BUTTON
  Button_Config();
#endif
#if TEST_DS1307
  MD_Config();
#endif
#if TEST_ADC
  Sensor_Config();
#endif
#if TEST_MAX6369
  Max6369_Config();
#endif
#if TEST_SIM800L
  delay(10000);
  Sim800L_Setup();
#endif
#if DEBUG_WEB
  WebSerial.println("Complete Setup");
#endif
}

void loop() {
#if TEST_MODBUS
  MB_Slave_Run();
#endif
#if TEST_BUTTON
  // Button_Blink();
  Button_Scan();
#endif
#if TEST_DS1307
  MD_Run();
#endif
#if TEST_ADC
  Sensor_Run();
#endif
#if TEST_MAX6369
  Max6369_Run();
#endif
#if TEST_SIM800L
  Sim800L_Read_SMS();
#endif

  delay(1);
}

