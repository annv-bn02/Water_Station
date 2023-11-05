#ifndef _MY_WEB_DEBUG_
#define _MY_WEB_DEBUG_

#include "all_header.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WebSerial.h>
#include <AsyncElegantOTA.h>
void WD_update();
void WD_Setup();
void WD_State();
void WD_Handler(uint8_t *data, size_t len);
#endif



