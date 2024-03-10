#ifndef SPIFFSUTIL_H
#define SPIFFSUTIL_H 

#include "webConfigData.h"

void initFFS();

void setWorkSerialType(bool t);//设置板子工作模式,false(默认串口)

bool getWorkSerialType();      //读取上次设置的使用方式

bool isLastWifiOK();//读取上一次是否成功连接wifi,当上一次成功是,本次开机就不启动配网功能而是直接连接路由器

void saveWifi(char* ssid,char* pwd);//保存wifi密码

void getWifi(char* ssid,char* pwd);//读取wifi密码

void cleanWifi(); //清除之前的wifi

#endif
