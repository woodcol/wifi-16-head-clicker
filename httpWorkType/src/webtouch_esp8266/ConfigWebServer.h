#ifndef CONFIGWEBSERVER_H
#define CONFIGWEBSERVER_H 
#include "webConfigData.h"

#ifdef ESP32
#include <WebServer.h>
//获取web服务器指针对象,在服务器没有运行的时可以绑定新相应方法
WebServer* getWebServerPtr(); 
#else
#include <ESP8266WebServer.h>
//获取web服务器指针对象,在服务器没有运行的时可以绑定新相应方法
ESP8266WebServer* getWebServerPtr(); 
#endif
void ledBlink(int n);
void handleNotFound();
void handleWebServer();
void initConfigWebServer(); //初始化web服务器为配网页面
void initWorkWebServer();   //初始化web服务器为工作页面
bool isWebServerRunning();//当前web服务器是否运行



#endif