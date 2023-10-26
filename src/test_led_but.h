#ifndef _TESTLED_
#define _TESTLED_

#include <Arduino.h>
#include "all_header.h"
#define LED1 5
#define LED2 18
#define LED3 19

#define BUT1 32
#define BUT2 33

void TLB_Config(void);
void TLB_Blink(void);
void TLB_Button(void);

#endif
