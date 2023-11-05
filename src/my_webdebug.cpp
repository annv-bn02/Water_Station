#include "my_webdebug.h"

const char* ssid = "artng1";
const char* password = "20022002";
AsyncWebServer server(80);
AsyncWebServer server_ota(81);
/* setup function */
void WD_Setup(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // Serial.printf("WiFi Failed!\n");
    return;
  }
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());

  WebSerial.begin(&server);
  WebSerial.msgCallback(WD_Handler);
  server.begin();
  server_ota.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I ESP32.");
  });

  AsyncElegantOTA.begin(&server_ota);    // Start ElegantOTA
  server_ota.begin();
  // Serial.println("HTTP server started");
}

void WD_update(void) {
  // server.handleClient();
}

void WD_State()
{
  // count_state++;
  // if (millis() > count_state + 10000) 
  // {
  //   count_state = millis();
  //   WebSerial.println(state);
  // }
}

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
