#ifndef _SIM800L_
#define _SIM800L_

#include "all_header.h"

extern String message_receive;
extern String phone_number;
extern char module_sim_number[11];
extern char phone_numner_sender[11];
void Sim800L_Setup(void);
void Sim800L_Update(void);
void Sim800L_Send_SMS(String number_receiver, String data);
uint8_t Sim800L_Read_SMS(void);
void Sim800L_Setup_ReadSMS(void);
uint8_t Sim800L_Status(void);
uint8_t Sim800L_Ping_Status(void);
uint8_t* Sim800L_Phone_Sender(void);
void Sim800L_Check_All_Flag(void);
#endif
