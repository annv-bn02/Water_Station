#ifndef _TESTADC_
#define _TESTADC_

#include <Arduino.h>
#include "all_header.h"
#define SENSOR_PIN 4
extern String sensor;
void TA_Config(void);
void TA_Run(void);
float TA_Sensor_Data(void);
#endif
