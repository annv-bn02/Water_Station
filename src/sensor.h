#ifndef _SENSOR_
#define _SENSOR_

#include <Arduino.h>
#include "all_header.h"
#define SENSOR_PIN 4
extern String sensor;
void Sensor_Config(void);
void Sensor_Run(void);
float Sensor_Sensor_Data(void);
#endif
