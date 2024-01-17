#ifndef _SENSOR_
#define _SENSOR_

#include <Arduino.h>
#include "all_header.h"

#include <SimpleKalmanFilter.h>
#include <stdio.h>
#include <stdint.h>

#define SAMPLE        60
#define ADC_PIN 33
extern String sensor;

extern float a_param, b_param, c_param, d_param, e_param;

void LengthSensor_KalmanFilter_Read(void);
void LengthSensor_Average_Read(void);
void Sensor_Config(void);
void Sensor_Run(void);
float Sensor_Sensor_Data(void);
#endif
