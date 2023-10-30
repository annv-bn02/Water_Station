#include "test_sim800l.h"

String message_receive;
String phone_number;

void TS_Setup()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  // delay(10000);
  // digitalWrite(25, HIGH);
  // digitalWrite(26, HIGH); 
  message_receive = "Setup";
  phone_number = "0";
  OTA_update();
  Serial.println("AT");
  TS_Update();
  Serial.println("AT+CSQ");
  TS_Update();
  Serial.println("AT+CCID");
  TS_Update();
  Serial.println("AT+CREG?");
  TS_Update();
  Serial.println("ATI");
  TS_Update();
  Serial.println("AT+CBC");
  TS_Update();
  TS_Send_SMS();
  TS_Setup_ReadSMS();
  message_receive = "Setup complete";
  phone_number = "0";
  OTA_update();
}
void TS_Update()
{
  delay(1000);
  String tmp;
  // while (Serial2.available())
  // {
  //   Serial.write(Serial2.read());//Forward what Serial2 received to Software Serial2 Port
  // }
  while (Serial.available())
  {
    phone_number = Serial.readString();//Forward what Software Serial2 received to Serial2 Port
  }
}
void TS_Send_SMS()
{
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  // delay(500);
  TS_Update();
  Serial.println("AT+CMGS=\"+84989691022\"");//947069655
  // delay(500);
  TS_Update();
  Serial.print("Circuit Test"); //text content
  // delay(500);
  TS_Update();
  // digitalWrite(25, HIGH);
  // digitalWrite(26, HIGH); 
  // Serial2.println();
  // Serial2.println("Message Send");
  // digitalWrite(25, LOW);
  // digitalWrite(26, LOW); 
  Serial.write(26);
}

void TS_Setup_ReadSMS(void)
{
  Serial.print("AT+IPR=115200\r\n");       // Cau hinh Baud Rate
  TS_Update();
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  TS_Update();
  Serial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  TS_Update();
  // Serial2.print("AT&W\r\n");                // Luu cau hinh
  // TS_Update();
}

uint8_t TS_Read_SMS(void)
{
  // message_receive = String(random(1, 100));
  // phone_number = String(random(1, 100));
  while (Serial.available())
  {
    String sms = Serial.readString();
    // Serial2.println(sms);
    // Serial.println(sms.length());
    sms = sms.substring(2);
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
  } 
  return 1;
}
