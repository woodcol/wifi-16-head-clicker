#ifndef INDEXHTML_H
#define INDEXHTML_H
#include "webConfigData.h"

#ifdef ESP32
#include <WebServer.h>

void sendWifiConfigHtml(WebServer *cli);
void sendWifiWorkHtml(WebServer *cli);

#else
#include <ESP8266WebServer.h>
void sendWifiConfigHtml(ESP8266WebServer *cli);
void sendWifiWorkHtml(ESP8266WebServer *cli);
#endif


#endif
