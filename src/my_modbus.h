#ifndef _MYMODBUS_
#define _MYMODBUS_

#include "all_header.h"
#include <modbus_rtu_library/ModbusRTU.h>

#define SLAVE_ID 1
#define START_REGISTER_USED 0
#define NUMBER_REGISTER_USED 227

#define SENSOR_DATA_START_REGISTER 0
#define BUTTON_DATA_START_REGISTER 32
#define BUTTON_CHANGE_START_REGISTER 40
#define STATUS_START_REGISTER 41
#define PING_STATUS_START_REGISTER 42
#define CALIB_PARAMETERS_START_REGISTER 48
#define GET_STATUS_START_REGISTER 54
#define REQUEST_CONFIG_START_REGISTER 60

#define LED_BLINK_START_REGISTER 71
#define PING_RESPONSE_START_REGISTER 74
#define CONFIG_FREQUENCE_START_REGISTER 109
#define RESPONSE_STATUS_START_REGISTER 111
#define CONFIG_LOCATION_START_REGISTER 140
#define CONFIG_MQTT_SEVER_START_REGISTER 186

#define SENSOR_NUMBER   16
#define BUTTON_NUMBER   16

#define DEFAULT_NUMBER_DATA 5
#define DEFAULT_DATA_START 6

#define RS485_TE 25
#define RS485_RE 26 

typedef enum{
    LB_LED_NUMBER_STATE     = 0,
    LB_TIME_ON_STATE        = 1,
    LB_TIME_OFF_STATE       = 2,
}led_blink_message_e;

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
    RS_NUMBER_MQTT_SUCCESS_STATE    = 7,
    RS_TIME_MQTT_STATE              = 8,
    RS_MQTT_STATUS_STATE            = 9,
}respone_status_message_e;

typedef enum{
    CL_LONGITUDE_STATE  = 0,
    CL_LATITUDE_STATE   = 1,
}config_location_message_e;

typedef enum{
    CM_MQTT_ADDRESS_STATE   = 0,
    CM_MQTT_PORT_STATE      = 1,
    CM_USER_STATE           = 2,
    CM_PASSWORD_STATE       = 3,
    CM_DATA_RECEIVE_STATE   = 4,
    CM_DATA_SEND_STATE      = 5,
}config_mqtt_message_e;

void MB_Slave_Run(void);
void MB_Slave_Setup(void);
uint16_t MB_Slave_Read_Register(float data);
void MB_Slave_Write_Register(uint16_t * array, uint16_t length, uint16_t reg_offset);


void MB_Slave_Write_Sensor(float *data);
void MB_Slave_Write_Button_State(uint8_t *data);
void MB_Slave_Write_Button_Change(uint16_t data);
void MB_Slave_Write_Status(uint16_t data);
void MB_Slave_Write_Ping_Status(uint16_t data, String phone_number);
void MB_Slave_Write_Calib_Parameters(float a, float b, float c);
void MB_Slave_Write_Get_Status(uint16_t data, String phone_number);
void MB_Slave_Write_Request_Config(uint16_t data);

void MB_Slave_Read_Led_Logic(uint8_t *data);
void MB_Slave_Read_Led_Blink(uint8_t *data);
void MB_Slave_Read_Ping_Response(uint8_t *data);
void MB_Slave_Read_Update_Frequence(uint8_t *data);
void MB_Slave_Read_Response_Status(uint8_t *data);
void MB_Slave_Read_Config_Location(uint8_t *data);
void MB_Slave_Read_Config_MQTT_Server(uint8_t *data);

uint8_t MB_AI_Read_Ping_Status(void);
uint8_t MB_AI_Read_Config_Para(void);
uint8_t MB_AI_Read_Get_Status(void);

//USED in ModbusRTU.cpp in modbus-esp8266 library
void MB_Slave_Filter_Read_Message(uint8_t *data);
void MB_Slave_Filter_Read_Multi_Register(uint8_t *data);
void MB_Slave_RS485_Write(void);
void MB_Slave_RS485_Read(void);
#endif
