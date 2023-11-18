#ifndef _SIM800L_
#define _SIM800L_

#include "all_header.h"

extern String message_receive;
extern String phone_number;
extern char module_sim_number[11];
extern char phone_numner_sender[11];
void TS_Setup(void);
void TS_Update(void);
void TS_Send_SMS(String number_receiver, String data);
uint8_t TS_Read_SMS(void);
void TS_Setup_ReadSMS(void);
uint8_t TS_Status(void);
uint8_t TS_Ping_Status(void);
uint8_t* TS_Phone_Sender(void);
void TS_Check_All_Flag(void);
#endif
