#include "my_modbus.h"

#define DEBUG_MODBUS 0

ModbusRTU mb;
uint16_t arr[100];
uint8_t button_data[100], rtc_data[10], ai_read_ping_status_flag = 0, ai_read_config_para_flag = 0, ai_read_get_status_flag = 0;
float sensor_data[100];

static void MB_Slave_Debug_Data(uint8_t *data, uint16_t length);
static void MB_Slave_Debug_Data_Char(char *data, uint16_t length);

/**
 * @brief Push new data to the register for readponse when Master reads data
 * 
 */
void MB_Slave_Run(void)
{
  sensor_data[0] = TA_Sensor_Data();
  for(int i = 1; i < SENSOR_NUMBER; i++)
  {
    sensor_data[i] = 100 + 0.5;
  }
  for(int i = 0; i < BUTTON_NUMBER; i++)
  {
    button_data[i] = 100;
  }
  // rtc_data[7] = 0;
  // for(int i = 0; i < 7; i++)
  // {
  //   rtc_data[i] = md_time[i];
  // }
  MB_Slave_Write_Sensor(sensor_data);
  MB_Slave_Write_RTC(md_time);
  MB_Slave_Write_Button_State(button_data);
  MB_Slave_Write_Button_Change(1);
  MB_Slave_Write_Status(TS_Status());
  MB_Slave_Write_Calib_Parameters(a_param, b_param, c_param, d_param, e_param);
  MB_Slave_Write_Get_Status(TS_Status(), module_sim_number);
  MB_Slave_Write_Ping_Status(TS_Ping_Status(), phone_numner_sender);
  mb.task();
  yield();
}

/**
 * @brief Setup for Modbus connection
 * Setup for using module RS485 for Modbus communication
 */
void MB_Slave_Setup(void)
{
  Serial2.begin(9600);
  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);
  mb.addHreg(START_REGISTER_USED, 0, NUMBER_REGISTER_USED);

  //Config RS485 Control
  pinMode(RS485_TE, OUTPUT);
  pinMode(RS485_RE, OUTPUT);
  MB_Slave_RS485_Read();

}

/**
 * @brief example for read data from a register
 * @param reg_address : register address to be readed
 */
uint16_t MB_Slave_Read_Register(uint16_t reg_address)
{
  uint16_t  temp;
  mb.Hreg(reg_address, temp);
  return temp;
}

/**
 * @brief example for write data to a register
 * 
 * @param array : array data to be written
 * @param length : length of the array
 * @param reg_offset : start address of the registers to be written
 */
void MB_Slave_Write_Register(uint16_t * array, uint16_t length, uint16_t reg_offset)
{
  for(int i = 0; i < length; i++)
  {
    mb.Hreg(reg_offset + i, array[i]);
  }
}

/**
 * @brief Write sensors data to the register for Master read
 * Data start write from SENSOR_DATA_START_REGISTER
 * @param data : array of sensors data
 */
void MB_Slave_Write_Sensor(float *data)
{
  for(int i = 0; i < SENSOR_NUMBER; i++)
  {
    uint16_t *temp_data = Convert_From_Float_To_Uint16(data[i]);
    mb.Hreg(SENSOR_DATA_START_REGISTER + i*2, temp_data[0]);
    mb.Hreg(SENSOR_DATA_START_REGISTER + i*2 + 1, temp_data[1]);
  }
}
/**
 * @brief Write Real Time readed from DS1307 to the register for Master read
 * 
 * @param data : array of time values
 */
void MB_Slave_Write_RTC(uint8_t *data)
{
  for(int i = 0; i < 8; i += 2)
  {
    uint16_t temp_data = Convert_From_Bytes_To_Uint16(data[i], data[i + 1]);
    mb.Hreg(RTC_DATA_START_REGISTER + i/2, temp_data);
  }  
}

/**
 * @brief Write buttons data to the register for Master read
 * Data start write from BUTTON_DATA_START_REGISTER
 * @param data : array of buttons data
 */
void MB_Slave_Write_Button_State(uint8_t *data)
{
  for(int i = 0; i < BUTTON_NUMBER; i += 2)
  {
    uint16_t temp_data = Convert_From_Bytes_To_Uint16(data[i], data[i + 1]);
    mb.Hreg(BUTTON_DATA_START_REGISTER + i/2, temp_data);
  }
}

/**
 * @brief Writes the information about the change of button state for Master read
 * Data start write from BUTTON_CHANGE_START_REGISTER
 * @param data : the change of button state
 */
void MB_Slave_Write_Button_Change(uint16_t data)
{
  mb.Hreg(BUTTON_CHANGE_START_REGISTER, data);
}

/**
 * @brief Write the status about have message or ping status for Master read
 * Data start write from STATUS_START_REGISTER
 * @param data : status
 */
void MB_Slave_Write_Status(uint16_t data)
{
  mb.Hreg(STATUS_START_REGISTER, data);
}

/**
 * @brief Write about ping status to the SIM800L for Master read
 * Data start write from PING_STATUS_START_REGISTER
 * @param data : ping status
 * @param phone_number : phone number send ping status message
 */
void MB_Slave_Write_Ping_Status(uint16_t data, String phone_number)
{
  mb.Hreg(PING_STATUS_START_REGISTER, data);
  for(int i = 0; i < 10; i += 2)
  {
    uint16_t temp_data = Convert_From_Bytes_To_Uint16(phone_number[i], phone_number[i + 1]);
    mb.Hreg(PING_STATUS_START_REGISTER + i/2 + 1, temp_data);
  }
}

/**
 * @brief Write calib parameters for Master read
 * Data start write from CALIB_PARAMETERS_START_REGISTER
 * @param a : first parameter
 * @param b : second parameter
 * @param c : third parameter
 * @param d : fourth parameter
 * @param e : fifth parameter
 */
void MB_Slave_Write_Calib_Parameters(float a, float b, float c, float d, float e)
{
  uint16_t *temp_data1 = Convert_From_Float_To_Uint16(a);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 0, temp_data1[0]);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 1, temp_data1[1]);
  uint16_t *temp_data2 = Convert_From_Float_To_Uint16(b);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 2, temp_data2[0]);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 3, temp_data2[1]);
  uint16_t *temp_data3 = Convert_From_Float_To_Uint16(c);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 4, temp_data3[0]);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 5, temp_data3[1]);
  uint16_t *temp_data4 = Convert_From_Float_To_Uint16(d);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 6, temp_data4[0]);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 7, temp_data4[1]);
  uint16_t *temp_data5 = Convert_From_Float_To_Uint16(e);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 8, temp_data5[0]);
  mb.Hreg(CALIB_PARAMETERS_START_REGISTER + 9, temp_data5[1]);
}

/**
 * @brief Write the 
 * Data start write from GET_STATUS_START_REGISTER
 * @param data 
 * @param phone_number 
 */
void MB_Slave_Write_Get_Status(uint16_t data, String phone_number)
{
  mb.Hreg(GET_STATUS_START_REGISTER, data);
}

/**
 * @brief Read ping response when Master write and send it from SIM800L
 * Data about phone number receive ping reponse and data of message;
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Ping_Response(uint8_t *data)
{
  ping_respone_message_e message_state;
  uint16_t number_byte = data[DEFAULT_NUMBER_DATA], temp_position = 0;
  uint8_t phone_number[10], ping_response_data[60];
  message_state = PR_PHONE_NUMBER_STATE;
  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i ++)
  {
    switch (message_state)
    {
      case PR_PHONE_NUMBER_STATE:
        phone_number[temp_position++] = data[i];
        if(temp_position == 10)
        {
          temp_position = 0;
          message_state = PR_DATA_STATE;
        }
        break;
      case PR_DATA_STATE:
        ping_response_data[temp_position++] = data[i];
        if(temp_position == 60)
        {
          temp_position = 0;
          message_state = PR_PHONE_NUMBER_STATE;
        }
        break;
      default:
        break;
    }
  }
  // TS_Send_SMS(String(phone_number), String(ping_response_data));
#if DEBUG_MODBUS
  Serial.print("Ping Resonse Phone Number: ");
  MB_Slave_Debug_Data(phone_number, 10);
  Serial.println();
  Serial.print("Ping Resonse Data: ");
  MB_Slave_Debug_Data(ping_response_data, 60);
  Serial.println();
#endif
}


/**
 * @brief Read response status when Master write
 * Data about location, phone number, mac_address, pin current rate, sim current state, time board reset, mqtt state,
 time disconnect mqtt server, number of successful mqtt messages, succcessful mqtt messages, ping time, sensor current value
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Response_Status(uint8_t *data)
{
  respone_status_message_e message_state;
  uint16_t number_byte = data[DEFAULT_NUMBER_DATA], temp_position = 0, mqtt_disconnect_time;
  char location[20];
  uint8_t phone_number[10], mac_address[6], pin_rate, sim_rate, reset_time;
  float sensor_current;
  uint32_t ping_time;
  message_state = RS_PHONE_NUMBER_STATE;

  uint8_t temp_data[4];

  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i ++)
  {
    switch (message_state)
    {
      case RS_PHONE_NUMBER_STATE:
        phone_number[temp_position++] = data[i];
        if(temp_position == 10)
        {
          temp_position = 0;
          message_state = RS_MAC_ADDRESS_STATE;
        }
        break;
      case RS_MAC_ADDRESS_STATE:
        mac_address[temp_position++] = data[i];
        if(temp_position == 6)
        {
          temp_position = 0;
          message_state = RS_LOCATION_STATE;
        }
        break;
      case RS_LOCATION_STATE:
        location[temp_position++] = data[i];
        if(temp_position == 20)
        {
          temp_position = 0;
          message_state = RS_SENSOR_DATA_STATE;
        }
        break;
      case RS_SENSOR_DATA_STATE:
        uint8_t temp_data[4];
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          sensor_current = Convert_From_Bytes_To_Float(temp_data[2], temp_data[3], temp_data[0], temp_data[1]);
          temp_position = 0;
          message_state = RS_PIN_SIM_STATE;
        }
        break;
      case RS_PIN_SIM_STATE:
        if(temp_position == 0)
        {
          pin_rate = data[i];
          temp_position = 1;
        }
        else if(temp_position == 1)
        {
          sim_rate = data[i];
          temp_position = 0;
          message_state = RS_INTERNET_STATUS_STATE;
        }
        break;
      case RS_INTERNET_STATUS_STATE:
        // uint8_t temp_data[4];
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          ping_time = Convert_From_Bytes_To_Int(temp_data[2], temp_data[3], temp_data[1], temp_data[0]);
          temp_position = 0;
          message_state = RS_DISCONNECT_TIME_STATE;
        }
        break;
      case RS_DISCONNECT_TIME_STATE:
        // uint8_t temp_data[2];
        temp_data[temp_position++] = data[i];
        if(temp_position == 2)
        {
          mqtt_disconnect_time = Convert_From_Bytes_To_Uint16(temp_data[1], temp_data[0]);
          temp_position = 0;
          message_state = RS_RESET_TIME_STATE;
        }
        break;
      case RS_RESET_TIME_STATE:
        reset_time = data[i];
        message_state = RS_PHONE_NUMBER_STATE;
        break;
      default:
        break;
    }
  }
  WebSerial.println("Resonse Status Sensor Data: " + String(sensor_current));
  WebSerial.println("Resonse Status Pin Rate: " + String(pin_rate));
  WebSerial.println("Resonse Status Sim Rate: " + String(sim_rate));
  WebSerial.println("Resonse Status Ping Time: " + String(ping_time));
  WebSerial.println("Resonse Status MQTT Disconnect Time: " + String(mqtt_disconnect_time));
  WebSerial.println("Resonse Status Reset Time: " + String(reset_time));
#if DEBUG_MODBUS
  Serial.print("Resonse Status Phone Number: ");
  MB_Slave_Debug_Data(phone_number, 10);
  Serial.println();
  Serial.print("Resonse Status Mac Address: ");
  MB_Slave_Debug_Data(mac_address, 6);
  Serial.println();
  Serial.print("Resonse Status Location: ");
  MB_Slave_Debug_Data_Char(location, 20);
  Serial.println();
  Serial.println("Resonse Status Sensor Data: " + String(sensor_current));
  Serial.println("Resonse Status Pin Rate: " + String(pin_rate));
  Serial.println("Resonse Status Sim Rate: " + String(sim_rate));
  Serial.println("Resonse Status Ping Time: " + String(ping_time));
  Serial.println("Resonse Status MQTT Disconnect Time: " + String(mqtt_disconnect_time));
  Serial.println("Resonse Status Reset Time: " + String(reset_time));
#endif
}

void MB_Slave_Read_Config_Param(uint8_t *data)
{
  config_param_e message_state;
  float a, b, c, d, e;
  message_state = A_PARAM;
  uint32_t number_byte = data[DEFAULT_NUMBER_DATA], temp_position = 0;
  uint8_t temp_data[4];
  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i ++)
  {
    switch (message_state)
    {
      case A_PARAM:
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          a = Convert_From_Bytes_To_Float(temp_data[2], temp_data[3], temp_data[0], temp_data[1]);
          temp_position = 0;
          message_state = B_PARAM;
        }
        break;
      case B_PARAM:
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          b = Convert_From_Bytes_To_Float(temp_data[2], temp_data[3], temp_data[0], temp_data[1]);
          temp_position = 0;
          message_state = C_PARAM;
        }
        break;
      case C_PARAM:
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          c = Convert_From_Bytes_To_Float(temp_data[2], temp_data[3], temp_data[0], temp_data[1]);
          temp_position = 0;
          message_state = D_PARAM;
        }
        break;
      case D_PARAM:
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          d = Convert_From_Bytes_To_Float(temp_data[2], temp_data[3], temp_data[0], temp_data[1]);
          temp_position = 0;
          message_state = E_PARAM;
        }
        break;
      case E_PARAM:
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          e = Convert_From_Bytes_To_Float(temp_data[2], temp_data[3], temp_data[0], temp_data[1]);
          temp_position = 0;
          message_state = A_PARAM;
        }
        break;
      default:
        break;
    }
  }
  a_param = a, b_param = b, c_param = c, d_param = d, e_param = e;
#if DEBUG_WEB
  WebSerial.printf("%.2f %.2f %.2f %.2f %.2f\n", a, b, c, d, e);
#endif
}

/**
 * @brief Function custom for reading message Master write
 * USED in ModbusRTU.cpp in modbus-esp8266 library
 * @param data : message data Master write
 */
void MB_Slave_Filter_Read_Message(uint8_t *data)
{
  uint8_t function = data[0];
  switch(function)
  {
    case 0x10:
      MB_Slave_Filter_Read_Multi_Register(data);
      break;
    case 0x03:
      if(Convert_From_Bytes_To_Uint16(data[2], data[1]) == PING_STATUS_START_REGISTER)
      {
        ai_read_ping_status_flag = 1;
#if DEBUG_WEB
        WebSerial.println("AI read ping status");
#endif
      }
      else if(Convert_From_Bytes_To_Uint16(data[2], data[1]) == CALIB_PARAMETERS_START_REGISTER)
      {
        ai_read_config_para_flag = 1;
#if DEBUG_WEB
        WebSerial.println("AI read config parameters");
#endif
      }
      else if(Convert_From_Bytes_To_Uint16(data[2], data[1]) == GET_STATUS_START_REGISTER)
      {
        ai_read_get_status_flag = 1;
#if DEBUG_WEB
        WebSerial.println("AI read get status");
#endif
      }
      break;
    default:
      break;
  }
}

/**
 * @brief Function for directing processing incoming message Mater write
 * 
 * @param data : data Master Write
 */
void MB_Slave_Filter_Read_Multi_Register(uint8_t *data)
{
  uint16_t start_address = Convert_From_Bytes_To_Uint16(data[2], data[1]);
  switch (start_address)
  {
  case PING_RESPONSE_START_REGISTER:
    MB_Slave_Read_Ping_Response(data);
    break;
  case RESPONSE_STATUS_START_REGISTER:
    MB_Slave_Read_Response_Status(data);
    break;
  case CONFIG_PARAM_START_REGISTER:
    MB_Slave_Read_Config_Param(data);
    break;
  default:
    break;
  }
}

/**
 * @brief Checking about status of Master(AI7688) read data Ping Status  
 * 
 * @return uint8_t : 1 if AI readed, 0 otherwise
 */
uint8_t MB_AI_Read_Ping_Status()
{
  if(ai_read_ping_status_flag == 1)
  {
    ai_read_ping_status_flag = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Checking about status of Master(AI7688) read data about config paparameters.  
 * 
 * @return uint8_t : 1 if AI readed, 0 otherwise
 */
uint8_t MB_AI_Read_Config_Para()
{
  if(ai_read_config_para_flag == 1)
  {
    ai_read_config_para_flag = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Checking about status of Master(AI7688) read data about get status.  
 * 
 * @return uint8_t : 1 if AI readed, 0 otherwise
 */
uint8_t MB_AI_Read_Get_Status()
{
  if(ai_read_get_status_flag == 1)
  {
    ai_read_get_status_flag = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Set up for writing data to the RS485
 * 
 */
void MB_Slave_RS485_Write(void)
{
  digitalWrite(RS485_TE, HIGH);
  digitalWrite(RS485_RE, HIGH);
}

/**
 * @brief Set up for reading data to the RS485
 * 
 */
void MB_Slave_RS485_Read(void)
{
  digitalWrite(RS485_TE, LOW);
  digitalWrite(RS485_RE, LOW);
}

/**
 * @brief Custom function for debuging data
 * 
 * @param data : array of bytes
 * @param length : length of array
 */
void MB_Slave_Debug_Data(uint8_t *data, uint16_t length)
{
  for(uint16_t i = 0; i < length; i++)
  {
    Serial.printf("%d ", data[i]);
  }
}

/**
 * @brief Custom function for debuging data
 * 
 * @param data : array of character data
 * @param length : length of array
 */
void MB_Slave_Debug_Data_Char(char *data, uint16_t length)
{
  for(uint16_t i = 0; i < length; i++)
  {
    Serial.printf("%c", data[i]);
  }
}
