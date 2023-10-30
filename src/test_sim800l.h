#ifndef _TEST_SIM800L_
#define _TEST_SIM800L_

#include "all_header.h"

extern String message_receive;
extern String phone_number;

void TS_Setup(void);
void TS_Update(void);
void TS_Send_SMS(void);
uint8_t TS_Read_SMS(void);
void TS_Setup_ReadSMS(void);
#endif
