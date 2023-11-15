#include "my_webdebug.h"

const char* ssid = "artng1";
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
  if(Data == "10112002")
  {
    ESP.restart();
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
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {

    return;
  }
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
