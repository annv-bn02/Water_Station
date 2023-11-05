#include "test_sim800l.h"

String message_receive = "1";
String phone_number = "1";
char module_sim_number[11] = "0966874225";
char phone_numner_sender[11];
uint8_t have_message = 0, have_config_para = 0, have_get_status = 0, have_ping_status = 0;
void TS_Setup()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  // delay(10000);
  // digitalWrite(25, HIGH);
  // digitalWrite(26, HIGH); 
  message_receive = "Setup";
  phone_number = "0";
  TS_Update();
  Serial.println("AT\r\n");
  TS_Update();
  Serial.println("AT+CSQ\r\n");
  TS_Update();
  Serial.println("AT+CCID\r\n");
  TS_Update();
  Serial.println("AT+CREG?\r\n");
  TS_Update();
  Serial.println("ATI\r\n");
  TS_Update();
  Serial.println("AT+CBC\r\n");
  TS_Update();
  // TS_Send_SMS();
  TS_Setup_ReadSMS();
  TS_Update();
}
void TS_Update()
{
  delay(100);
  String tmp;
  // while (Serial2.available())
  // {
  //   Serial.write(Serial2.read());//Forward what Serial2 received to Software Serial2 Port
  // }
  while (Serial.available())
  {
    tmp = Serial.readStringUntil('\n');//Forward what Software Serial2 received to Serial2 Port
  }
#if DEBUG_WEB
  WebSerial.println(tmp);
#endif
}
void TS_Send_SMS(String number_receiver, String data)
{
  number_receiver = number_receiver.substring(1);
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  TS_Update();
  Serial.println("AT+CMGS=\"+84" + number_receiver + "\"");//947069655
  TS_Update();
  Serial.print(data); //text content
  TS_Update();
  Serial.write(26);
#if DEBUG_WEB
  WebSerial.println("Send SMS from module sim number: " + String(module_sim_number) + " to: " + number_receiver);
  WebSerial.println("SMS: " + data);
#endif
}

void TS_Setup_ReadSMS(void)
{
  Serial.print("AT+IPR=115200\r\n");       // Cau hinh Baud Rate
  TS_Update();
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  TS_Update();
  Serial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  TS_Update();
  // Serial.print("AT&W\r\n");                // Luu cau hinh
  // TS_Update();
}

uint8_t TS_Read_SMS(void)
{
  while (Serial.available())
  {
    String sms = Serial.readString();
    // Serial2.println(sms);
    // Serial.println(sms.length());
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

uint8_t TS_Status(void)
{
  TS_Check_All_Flag();
  if(have_message == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t TS_Ping_Status(void)
{
  TS_Check_All_Flag();
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

void TS_Check_All_Flag(void)
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