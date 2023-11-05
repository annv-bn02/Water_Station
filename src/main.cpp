#include "all_header.h"
#define TEST_MODBUS 1
#define TEST_LED_BUT 1
#define TEST_DS1307 1
#define TEST_ADC 0
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
  delay(10000);
  TS_Setup();
#endif
#if DEBUG_WEB
  WebSerial.println("Complete Setup");
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

  delay(1);
}

