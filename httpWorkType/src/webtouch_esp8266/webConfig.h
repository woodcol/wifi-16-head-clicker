#ifndef WEBCONFIGINCLUDE_H
#define WEBCONFIGINCLUDE_H 

#include "webConfigData.h"
#include <WiFiClient.h>
#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiAP.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiAP.h>
#endif


#include "strUtil.h"
#include "indexhtml.h"
#include "LFSUtil.h"
#include "ConfigWebServer.h"
#include "APAndSTAMode.h"

#endif