#ifndef _MYMODBUS_
#define _MYMODBUS_

#include "all_header.h"
#include <modbus_rtu_library/ModbusRTU.h>

#define SLAVE_ID 1
#define START_REGISTER_USED 0
#define NUMBER_REGISTER_USED 146

#define SENSOR_DATA_START_REGISTER 0
#define BUTTON_DATA_START_REGISTER 20
#define RTC_DATA_START_REGISTER 25
#define BUTTON_CHANGE_START_REGISTER 30
#define STATUS_START_REGISTER 31
#define PING_STATUS_START_REGISTER 32
#define CALIB_PARAMETERS_START_REGISTER 38
#define GET_STATUS_START_REGISTER 48

#define PING_RESPONSE_START_REGISTER 74
#define RESPONSE_STATUS_START_REGISTER 110
#define CONFIG_PARAM_START_REGISTER 136
// #define LED_BLINK_START_REGISTER 61
// #define PING_RESPONSE_START_REGISTER 64
// #define CONFIG_FREQUENCE_START_REGISTER 99
// #define RESPONSE_STATUS_START_REGISTER 101
// #define CONFIG_LOCATION_START_REGISTER 130
// #define CONFIG_MQTT_SEVER_START_REGISTER 156

#define SENSOR_NUMBER   10
#define BUTTON_NUMBER   10

#define DEFAULT_NUMBER_DATA 5
#define DEFAULT_DATA_START 6

#define RS485_RTE 4 

typedef enum{
    PR_PHONE_NUMBER_STATE   = 0,
    PR_DATA_STATE           = 1,
}ping_respone_message_e;

typedef enum{
    RS_PHONE_NUMBER_STATE           = 0,
    RS_MAC_ADDRESS_STATE            = 1,
    RS_LOCATION_STATE               = 2,
    RS_SENSOR_DATA_STATE            = 3,
    RS_PIN_SIM_STATE                = 4,
    RS_INTERNET_STATUS_STATE        = 5,
    RS_DISCONNECT_TIME_STATE        = 6,
    RS_RESET_TIME_STATE             = 7,
}respone_status_message_e;

typedef enum{
    A_PARAM           = 0,
    B_PARAM           = 1,
    C_PARAM           = 2,
    D_PARAM           = 3,
    E_PARAM           = 4,
}config_param_e;

void MB_Slave_Run(void);
void MB_Slave_Setup(void);
uint16_t MB_Slave_Read_Register(float data);
void MB_Slave_Write_Register(uint16_t * array, uint16_t length, uint16_t reg_offset);


void MB_Slave_Write_Sensor(float *data);
void MB_Slave_Write_Button_State(uint8_t *data);
void MB_Slave_Write_RTC(uint8_t *data);
void MB_Slave_Write_Button_Change(uint16_t data);
void MB_Slave_Write_Status(uint16_t data);
void MB_Slave_Write_Ping_Status(uint16_t data, String phone_number);
void MB_Slave_Write_Calib_Parameters(float a, float b, float c, float d, float e);
void MB_Slave_Write_Get_Status(uint16_t data, String phone_number);

void MB_Slave_Read_Ping_Response(uint8_t *data);
void MB_Slave_Read_Response_Status(uint8_t *data);
void MB_Slave_Read_Config_Param(uint8_t *data);

uint8_t MB_AI_Read_Ping_Status(void);
uint8_t MB_AI_Read_Config_Para(void);
uint8_t MB_AI_Read_Get_Status(void);

//USED in ModbusRTU.cpp in modbus-esp8266 library
void MB_Slave_Filter_Read_Message(uint8_t *data);
void MB_Slave_Filter_10(uint8_t *data);
void MB_Slave_Filter_03(uint8_t *data);
void MB_Slave_RS485_Write(void);
void MB_Slave_RS485_Read(void);
#endif
