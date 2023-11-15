#ifndef _LENGTH_SENSOR_
#define _LENGTH_SENSOR_

#include <Arduino.h>
#include <SimpleKalmanFilter.h>
#include <stdio.h>
#include <stdint.h>

#define SAMPLE        60
#define ADC_PIN       4

void LengthSensor_Test(void);
void LengthSensor_KalmanFilter_Read(void);
void LengthSensor_Average_Read(void);
float LengthSensor_Return_Value(void);
void LengthSensor_Setup(void);

#endif