#ifndef __DATAUTIL_H__
#define __DATAUTIL_H__
#include <ESP8266WiFi.h>

/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

void beginEEPROM();
void endEEPROM();
void readDataFromEEPROM();
void saveDataToEEPROM(bool isEnd = true);
void initEEPROM();
void onEEPROMDataChange();
void setTouchType(uint8_t ptype);
bool isSetWifi();
bool isSetSerial();
void setWifiType(uint8_t ptype);
int char2int(char input);
void hex2bin(const char *src, uint8_t *target);

#endif /* INC_TOUCH_H_ */
