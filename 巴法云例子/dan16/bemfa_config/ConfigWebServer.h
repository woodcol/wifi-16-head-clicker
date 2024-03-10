#ifndef CONFIGWEBSERVER_H
#define CONFIGWEBSERVER_H 

#include <ESP8266WebServer.h>

void handleNotFound();
void handleWebServer();
void stopConfigWebServer();
void initConfigWebServer();

bool isWebServerRunning();//当前web服务器是否运行

//启动web服务器Handle,web服务器只能运行不能停止
//停止方法没有研究,没有写
bool startWebServerHandle();
bool stopWebServerAndClernHandle();

//获取web服务器指针对象,在服务器没有运行的时可以绑定新相应方法
ESP8266WebServer* getWebServerPtr(); 

#endif