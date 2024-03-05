#ifndef _WIND_
#define _WIND_

#include <Arduino.h>
#include "all_header.h"


void Wind_Config(void);
void Wind_Run(void);
void Wind_Speed(void );
void Wind_Convert_Direction(uint32_t dir_adc);
#endif
