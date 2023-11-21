#ifndef _BUTTON_
#define _BUTTON_

#include <Arduino.h>
#include "all_header.h"
#define LED1 5
#define LED2 18
#define LED3 19

#define BUTTON_1 32
#define BUTTON_2 33
#define BUTTON_3 32
#define BUTTON_4 32
#define BUTTON_5 32

typedef struct{
	unsigned char vruc_CurrentStt;
	unsigned char vruc_OldStt;
	unsigned char vruc_CountStt;
	unsigned char vruc_Flag;
} button_s;

extern uint8_t button_state_change;
void Button_Config(void);
void Button_Blink(void);
uint8_t Button_Read(button_s *button_struct, unsigned char button_read);
void Button_Scan(void);

#endif
