#include "sim800l.h"

String message_receive = "1";
String phone_number = "1";
char module_sim_number[11] = "0966874225";
char phone_numner_sender[11];
uint8_t have_message = 0, have_config_para = 0, have_get_status = 0, have_ping_status = 0;

/**
 * @brief Checking the state of SIM800L and Setup for receive messgages 
 * 
 */
void Sim800L_Setup()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  message_receive = "Setup";
  phone_number = "0";
#if DEBUG_WEB
  WebSerial.println("Start setup SIM800L");
  delay(1000);
#endif
  Sim800L_Update();
  Serial.println("AT\r\n");
  Sim800L_Update();
  Serial.println("AT+CSQ\r\n");
  Sim800L_Update();
  Serial.println("AT+CCID\r\n");
  Sim800L_Update();
  Serial.println("AT+CREG?\r\n");
  Sim800L_Update();
  Serial.println("ATI\r\n");
  Sim800L_Update();
  Serial.println("AT+CBC\r\n");
  Sim800L_Update();
  Sim800L_Setup_ReadSMS();
  Sim800L_Update();
}

/**
 * @brief Update information about the SIM800l respond
 * 
 */
void Sim800L_Update()
{
  delay(300);
  String tmp;
  // while (Serial2.available())
  // {
  //   Serial.write(Serial2.read());//Forward what Serial2 received to Software Serial2 Port
  // }

  //reading reponse from SIM800l for debuging
  while (Serial.available())
  {
    tmp = Serial.readStringUntil('\n');
  }
#if DEBUG_WEB
  WebSerial.println(tmp);
#endif
}

/**
 * @brief Send a message from SIM800L to the specified
 * 
 * @param number_receiver : the phone number receiver message
 * @param data : the message to send
 */
void Sim800L_Send_SMS(String number_receiver, String data)
{
  number_receiver = number_receiver.substring(1);
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  Sim800L_Update();
  Serial.println("AT+CMGS=\"+84" + number_receiver + "\"");//947069655
  Sim800L_Update();
  Serial.print(data); //text content
  Sim800L_Update();
  Serial.write(26);
#if DEBUG_WEB
  WebSerial.println("Send SMS from module sim number: " + String(module_sim_number) + " to: " + number_receiver);
  WebSerial.println("SMS: " + data);
#endif
}

/**
 * @brief Setup for receive message
 * 
 */
void Sim800L_Setup_ReadSMS(void)
{
  Serial.print("AT+IPR=115200\r\n");       // Cau hinh Baud Rate
  Sim800L_Update();
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  Sim800L_Update();
  Serial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  Sim800L_Update();
  Serial.print("AT&W\r\n");                // Luu cau hinh
  Sim800L_Update();
}

/**
 * @brief Read incoming SMS messages
 * Custom function for handling incoming messages to SIM800L
 * @return uint8_t : nothing until incoming code version
 */
uint8_t Sim800L_Read_SMS(void)
{
  while (Serial.available())
  {
    String sms = Serial.readString();
#if DEBUG_WEB
    WebSerial.println(sms);
    WebSerial.println(sms.length());
#endif
    sms = sms.substring(2);
    if(sms.substring(0, 4) == "+CMT")
    {
      String phone_sender = "0" + sms.substring(sms.indexOf("\"+84") + 4, sms.indexOf("\",\""));
      // Serial2.println(phone_number);
      int index = sms.indexOf("\r\n");
      sms = sms.substring(index + 2);
      index = sms.indexOf("\r\n");
      String message = sms.substring(0, index);
      // index = message.indexOf('\n');
      // message = message.substring(index);
      // Serial2.println("Message is :" + message );
      // Serial.println(message.length());
      message_receive = message;
      phone_number = phone_sender;
      if(message_receive == "ping")
      {
        have_ping_status = 1;
      }
      have_message = 1;
#if DEBUG_WEB
      WebSerial.println("Receive SMS from: " + phone_number + " to module sim number: " + String(module_sim_number));
      WebSerial.println("SMS: " + message_receive);
#endif
    }
  } 
  return 1;
}

/**
 * @brief Checking the status about the message received or ping status
 * 
 * @return uint8_t : 1 if have message received or ping status, 0 otherwise
 */
uint8_t Sim800L_Status(void)
{
  Sim800L_Check_All_Flag();
  if(have_message == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Checking for ping status
 * 
 * @return uint8_t : 1 if have ping status, 0 otherwise
 */
uint8_t Sim800L_Ping_Status(void)
{
  Sim800L_Check_All_Flag();
  if(have_ping_status == 1)
  {
    phone_number.toCharArray(phone_numner_sender, 11);
    return 1;
  }
  else
  {
    for(uint8_t i = 0; i < 10; i++)
    {
      phone_numner_sender[i] = 0;
    }
    return 0;
  }
}

/**
 * @brief Check all status for knowing state of Status 
 * 
 */
void Sim800L_Check_All_Flag(void)
{
  if(MB_AI_Read_Ping_Status() == 1)
  {
#if DEBUG_WEB
    WebSerial.println("Receive SMS from: " + phone_number + " to module sim number: " + String(module_sim_number));
    WebSerial.println("SMS: " + message_receive);
#endif
    have_ping_status = 0;
  }
  if(MB_AI_Read_Get_Status() == 1)
  {
    have_get_status = 0;
  }
  if(MB_AI_Read_Config_Para() == 1)
  {
    have_config_para = 0;
  }
  if(have_config_para == 0 && have_get_status == 0 && have_ping_status == 0)
  {
    have_message = 0;
  }
  else
  {
    have_message = 1;
  }
}