#include "my_webdebug.h"

const char* ssid = "artng2";
const char* password = "20022002";
const char* hostname = "water";
AsyncWebServer server(80);
AsyncWebServer server_ota(81);

/**
 * @brief Handle for receive data from WebSerial server
 * 
 * @param data : data to be received
 * @param len : length of data to be received
 */
void WD_Handler(uint8_t *data, size_t len){
  WebSerial.println("Data Received!");
  String Data = "";
  for(int i=0; i < len; i++){
    Data += char(data[i]);
  }
  WebSerial.println(Data);
  //If send string "10112002" in WebSerial ESP32 reset
  if(Data == "10112002")
  {
    WebSerial.println("Esp32 reset after 1 second");
    delay(1000);
    ESP.restart();
  }
  //If send String "testsendsms" in WebSerial SIM800L send sms for testting
  if(Data == "testsendsms")
  {
    WebSerial.println("Esp32 send sms");
    Sim800L_Send_SMS("0989691022", "Test send sms");
  }
  //If send String "setuptime" in WebSerial setup time for DS1307
  if(Data.substring(0, 9) == "setuptime")
  {
    WebSerial.println("Esp32 setup DS1307");
    MD_Setup_Time(Data);
  }
}

/**
 * @brief Setup
 * Connect to wifi
 * Connect to Sever for WebSerial
 * Connect to Sever for OTA 
 */
void WD_Setup(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult(15000) != WL_CONNECTED)
  //setup host name for easy connect without ip address
  //form: http://hostname.local
  if (!MDNS.begin(hostname)) { 
    while (1) {
      delay(1000);
    }
  }

  //setup for WebSerial, port 80, //http://hostname.local/WebSerial
  WebSerial.begin(&server);
  WebSerial.msgCallback(WD_Handler);
  server.begin();

  //setup for ota, port 81, //http://hostname.local:81/update
  server_ota.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I ESP32.");
  });
  AsyncElegantOTA.begin(&server_ota); 
  server_ota.begin();
}
