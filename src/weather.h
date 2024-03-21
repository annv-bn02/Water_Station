#ifndef _WEATHER_
#define _WEATHER_

#include <Arduino.h>
#include "all_header.h"

extern float wind_v, wind_d, rain_perhour;
void Weather_Config(void);
void Weather_Update(void);
void Weather_Wind_Speed(void );
void Weather_Wind_Convert_Direction(uint32_t dir_adc);
void Weather_Rain(void);
#endif
