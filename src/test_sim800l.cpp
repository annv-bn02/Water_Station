#include "test_sim800l.h"

void TS_Check()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  // delay(10000);
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
  TS_Setup_Receive();
}
void TS_Update()
{
  delay(100);
  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Serial2 received to Software Serial2 Port
  }
  while (Serial.available())
  {
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH); 
    Serial2.write(Serial.read());//Forward what Software Serial2 received to Serial2 Port
  }
  digitalWrite(25, LOW);
  digitalWrite(26, LOW); 
}
void TS_Send_SMS()
{
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  // delay(500);
  TS_Update();
  Serial.println("AT+CMGS=\"+84947069655\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  // delay(500);
  TS_Update();
  Serial.print("Circuit Test"); //text content
  // delay(500);
  TS_Update();
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH); 
  Serial2.println();
  Serial2.println("Message Send");
  digitalWrite(25, LOW);
  digitalWrite(26, LOW); 
  Serial.write(26);
}

void TS_Setup_Receive()
{
  Serial2.print("AT\r\n");                  // Kiem tra module con hoat dong khong
  delay(100);
  Serial2.print("AT+CSQ\r\n");              // Kiem tra chat luong song
  delay(100);
  Serial2.print("AT+IPR=115200\r\n");       // Cau hinh Baud Rate
  delay(100);
  Serial2.print("AT+CMGF=1\r\n");           // Cau hinh tin nhan dang text
  delay(100);
  Serial2.print("AT+CNMI=2,2,0,0,0\r\n");   // Chuyển đổi chế độ nhận tin nhắn
  delay(100);
  Serial2.print("AT+CNMI=2,2,0,0\r\n");     // Hien thi noi dung tin nhan
  delay(100);
  Serial2.print("AT+CLIP=1\r\n");           // Bật chức năng báo cuộc gọi đến
  delay(100);
  Serial2.print("AT+CUSD=1,\"*101#\"\r\n"); // Kiem tra so du tai khoan
  delay(100);
}

uint8_t Message_Receive()
{
    String data_receive = Serial.readStringUntil('\n');
    uint8_t length = data_receive.length() + 1; // Thêm kí hiệu '\0'
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH); 
    data_receive.trim(); 
    if(length > 1)
    {
      Serial2.println(data_receive);
      Serial2.println(length);
    }
    // if(length == 24) // Serial : 23
    // {
    //     data_receive.toCharArray(arr_receive, length);
    //     Serial.println();
    //     Serial.println((String)arr_receive);
    //     flag_receive = 1;
    // }
    return 1;
}
