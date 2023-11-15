#include "my_modbus.h"

#define DEBUG_MODBUS 0

ModbusRTU mb;
uint16_t arr[100];
uint8_t button_data[100], ai_read_ping_status_flag = 0, ai_read_config_para_flag = 0, ai_read_get_status_flag = 0;
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
  for(int i = 0; i < 20; i++)
  {
    if(i <= 6)
    {
      button_data[i] = md_time[i];
    }
    else
    {
      button_data[i] = 100;
    }
  }
  MB_Slave_Write_Sensor(sensor_data);
  MB_Slave_Write_Button_Change(1);
  MB_Slave_Write_Button_State(button_data);
  MB_Slave_Write_Status(TS_Status());
  MB_Slave_Write_Calib_Parameters(4.5, 5.5, 6.5);
  MB_Slave_Write_Get_Status(TS_Status(), module_sim_number);
  MB_Slave_Write_Ping_Status(TS_Ping_Status(), phone_numner_sender);
  MB_Slave_Write_Request_Config(1);
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
 */
void MB_Slave_Write_Calib_Parameters(float a, float b, float c)
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
 * @brief Write the status about the request config for Master read
 * Data start write from REQUEST_CONFIG_START_REGISTER
 * @param data 
 */
void MB_Slave_Write_Request_Config(uint16_t data)
{
  mb.Hreg(REQUEST_CONFIG_START_REGISTER, data);
}

/**
 * @brief Read led logic when Master write
 * 
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Led_Logic(uint8_t *data)
{
  uint8_t led_number = data[3], value = data[4];
#if DEBUG_MODBUS
  Serial.println("Led number: " + String(led_number));
  Serial.println("Value: " + String(value));
#endif
}

/**
 * @brief Read led link when Master write
 * Data about led_number and time on/off led;
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Led_Blink(uint8_t *data)
{
  led_blink_message_e message_state;
  uint16_t number_byte = data[DEFAULT_NUMBER_DATA], led_number, time_on, time_off;
  message_state = LB_LED_NUMBER_STATE;
  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i += 2)
  { 
    switch (message_state)
    {
      case LB_LED_NUMBER_STATE:
        led_number = Convert_From_Bytes_To_Uint16(data[i + 1], data[i]);
        message_state = LB_TIME_ON_STATE;
        break;
      case LB_TIME_ON_STATE:
        time_on = Convert_From_Bytes_To_Uint16(data[i + 1], data[i]);
        message_state = LB_TIME_OFF_STATE;
        break;
      case LB_TIME_OFF_STATE:
        time_off = Convert_From_Bytes_To_Uint16(data[i + 1], data[i]);
        message_state = LB_LED_NUMBER_STATE;
        break;
      default:
        break;
    }
  }
#if DEBUG_MODBUS
  Serial.println("Led blink Led Number: " + String(led_number) + " " + "Time on: " + String(time_on) + " " +  "Time off: " + String(time_off));
#endif
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
 * @brief Read update frequence when Master write
 * Data about frequence update data 
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Update_Frequence(uint8_t *data)
{
  uint16_t number_byte = data[DEFAULT_NUMBER_DATA], temp_position = 0;
  uint8_t frequen_arr[4];
  int32_t frequency;
  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i ++)
  {
    frequen_arr[temp_position++] = data[i];
  }
  frequency = Convert_From_Bytes_To_Int(frequen_arr[0], frequen_arr[1], frequen_arr[2], frequen_arr[3]);
#if DEBUG_MODBUS 
  Serial.println("Config frequency:" + String(frequency));
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
  uint8_t phone_number[10], mac_address[6], pin_rate, sim_rate, reset_time, mqtt_state;
  float sensor_current;
  uint32_t ping_time, number_mqtt_success, success_mqtt_time;
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
          sensor_current = Convert_From_Bytes_To_Float(temp_data[0], temp_data[1], temp_data[2], temp_data[3]);
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
          ping_time = Convert_From_Bytes_To_Int(temp_data[0], temp_data[1], temp_data[2], temp_data[3]);
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
          message_state = RS_NUMBER_MQTT_SUCCESS_STATE;
        }
        break;
      case RS_NUMBER_MQTT_SUCCESS_STATE:
        // uint8_t temp_data[4];
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          number_mqtt_success = Convert_From_Bytes_To_Int(temp_data[0], temp_data[1], temp_data[2], temp_data[3]);
          temp_position = 0;
          message_state = RS_TIME_MQTT_STATE;
        }
        break;
      case RS_TIME_MQTT_STATE:
        // uint8_t temp_data[4];
        temp_data[temp_position++] = data[i];
        if(temp_position == 4)
        {
          success_mqtt_time = Convert_From_Bytes_To_Int(temp_data[0], temp_data[1], temp_data[2], temp_data[3]);
          temp_position = 0;
          message_state = RS_MQTT_STATUS_STATE;
        }
        break;
      case RS_MQTT_STATUS_STATE:
        if(temp_position == 0)
        {
          reset_time = data[i];
          temp_position = 1;
        }
        else if(temp_position == 1)
        {
          mqtt_state = data[i];
          temp_position = 0;
          message_state = RS_PHONE_NUMBER_STATE;
        }
        break;
      default:
        break;
    }
  }
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
  Serial.println("Resonse Status Number MQTT Success: " + String(number_mqtt_success));
  Serial.println("Resonse Status Success MQTT Time: " + String(success_mqtt_time));
  Serial.println("Resonse Status Reset Time: " + String(reset_time));
  Serial.println("Resonse Status Mqtt State: " + String(mqtt_state));
#endif
}
/**
 * @brief Read update frequence when Master write
 * Data about location for configuration
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Config_Location(uint8_t *data)
{
  config_location_message_e message_state;
  uint16_t number_byte = data[DEFAULT_NUMBER_DATA], temp_position = 0;
  uint8_t longitude[4], latitude[4];
  message_state = CL_LONGITUDE_STATE;
  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i ++)
  {
    switch (message_state)
    {
      case CL_LONGITUDE_STATE:
        longitude[temp_position++] = data[i];
        if(temp_position == 4)
        {
          temp_position = 0;
          message_state = CL_LATITUDE_STATE;
        }
        break;
      case CL_LATITUDE_STATE:
        latitude[temp_position++] = data[i];
        if(temp_position == 4)
        {
          temp_position = 0;
          message_state = CL_LONGITUDE_STATE;
        }
        break;
      default:
        break;
    }
  }
#if DEBUG_MODBUS
  Serial.print("Config Location Longitude: ");
  MB_Slave_Debug_Data(longitude, 4);
  Serial.println();
  Serial.print("Config Location Latitude: ");
  MB_Slave_Debug_Data(latitude, 4);
  Serial.println();
#endif
}

/**
 * @brief Read update frequence when Master write
 * Data about configuration mqtt server about mqtt address, mqtt port, user and password of mqtt, data received from mqtt, data send to mqtt
 * @param data : data array cutting from Master write message buffer
 */
void MB_Slave_Read_Config_MQTT_Server(uint8_t *data)
{
  config_mqtt_message_e message_state;
  uint16_t number_byte = data[DEFAULT_NUMBER_DATA], temp_position = 0, port = 0;
  // Serial.println(number_byte);
  uint8_t mqtt_port[2];
  char mqtt_address[20], user[20], password[20], receive_data[20], send_data[20];
  message_state = CM_MQTT_ADDRESS_STATE;

  for(uint16_t i = DEFAULT_DATA_START; i < DEFAULT_DATA_START + number_byte; i++)
  {
    switch (message_state)
    {
      case CM_MQTT_ADDRESS_STATE:
        mqtt_address[temp_position++] = data[i];
        if(temp_position == 20)
        {
          temp_position = 0;
          message_state = CM_MQTT_PORT_STATE;
        }
        break;
      case CM_MQTT_PORT_STATE:
        mqtt_port[temp_position++] = data[i];
        if(temp_position == 2)
        {
          port = Convert_From_Bytes_To_Uint16(mqtt_port[1], mqtt_port[0]);
          temp_position = 0;
          message_state = CM_USER_STATE;
        }
        break;
      case CM_USER_STATE:
        user[temp_position++] = data[i];
        if(temp_position == 20)
        {
          temp_position = 0;
          message_state = CM_PASSWORD_STATE;
        }
        break;
      case CM_PASSWORD_STATE:
        password[temp_position++] = data[i];
        if(temp_position == 20)
        {
          temp_position = 0;
          message_state = CM_DATA_RECEIVE_STATE;
        }
        break;
      case CM_DATA_RECEIVE_STATE:
        receive_data[temp_position++] = data[i];
        if(temp_position == 20)
        {
          temp_position = 0;
          message_state = CM_DATA_SEND_STATE;
        }
        break;
      case CM_DATA_SEND_STATE:
        send_data[temp_position++] = data[i];
        if(temp_position == 20)
        {
          temp_position = 0;
          message_state = CM_MQTT_ADDRESS_STATE;
        }
        break;
      default:
        break;
    }
  }
#if DEBUG_MODBUS
  Serial.print("Config MQTT Sever Address: ");
  MB_Slave_Debug_Data_Char(mqtt_address, 20);
  Serial.println();
  Serial.print("Config MQTT Port: ");
  Serial.println(port);
  Serial.print("Config MQTT User: ");
  MB_Slave_Debug_Data_Char(user, 20);
  Serial.println();
  Serial.print("Config MQTT Password: ");
  MB_Slave_Debug_Data_Char(password, 20);
  Serial.println();
  Serial.print("Config MQTT Receive Data: ");
  MB_Slave_Debug_Data_Char(receive_data, 20);
  Serial.println();
  Serial.print("Config MQTT Send Data: ");
  MB_Slave_Debug_Data_Char(send_data, 20);
  Serial.println();
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
    case 0x06:
      MB_Slave_Read_Led_Logic(data);
      break;
    case 0x10:
      MB_Slave_Filter_Read_Multi_Register(data);
      break;
    case 0x03:
      if(Convert_From_Bytes_To_Uint16(data[2], data[1]) == 0x002a)
      {
        ai_read_ping_status_flag = 1;
#if DEBUG_WEB
        WebSerial.println("AI read ping status");
#endif
      }
      else if(Convert_From_Bytes_To_Uint16(data[2], data[1]) == 0x0030)
      {
        ai_read_config_para_flag = 1;
#if DEBUG_WEB
        WebSerial.println("AI read config parameters");
#endif
      }
      else if(Convert_From_Bytes_To_Uint16(data[2], data[1]) == 0x0036)
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
  case LED_BLINK_START_REGISTER:
    MB_Slave_Read_Led_Blink(data);
    break;
  case PING_RESPONSE_START_REGISTER:
    MB_Slave_Read_Ping_Response(data);
    break;
  case CONFIG_FREQUENCE_START_REGISTER:
    MB_Slave_Read_Update_Frequence(data);
    break;
  case RESPONSE_STATUS_START_REGISTER:
    MB_Slave_Read_Response_Status(data);
    break;
  case CONFIG_LOCATION_START_REGISTER:
    MB_Slave_Read_Config_Location(data);
    break;
  case CONFIG_MQTT_SEVER_START_REGISTER:
    MB_Slave_Read_Config_MQTT_Server(data);
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
