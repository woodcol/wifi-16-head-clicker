
#include "strUtil.h"
#include "webConfigData.h"
#include "APAndSTAMode.h"

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif



extern void digitalWrite(uint8_t pin, uint8_t val);

//给buff加上int型字符串,并返回加上字符串后的指令地址
char* addInt2Str(char* buff,int num)
{
    char *p = buff;
    p+=sprintf(p, "%d",num);
    return p;
}


//给buff加上int型字符串,并返回加上字符串后的指令地址
int uint8ToStrHex(char* buff,uint8_t num)
{
    return sprintf(buff, "%x",num);
}


char* addStr2Str(char* buff,char* addstr)
{
    char *p = buff;
    p+=sprintf(p, "%s",addstr);
    return p;
}

//将字符串s复制到t中,同时把s清空
void copyStr(char* s,char* t)
{
    while(*s){
        *t = *s;
        *s = 0;
        t++;
        s++;
    }
    *t = 0;
}

