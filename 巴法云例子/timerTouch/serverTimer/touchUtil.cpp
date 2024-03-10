#include <ESP8266WiFi.h>
#include "touchUtil.h"
#include "touch.h"
/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

bool isStart = false;

int touchTime  = 80;    //点击屏的时间,即模仿手指点击时在手屏上的停留时间，单位：毫秒

int delayTime  = 50;    //点击后停止时间，单位：毫秒

HC595Touch touchObj = HC595Touch();

extern char RCVBUFF[];

union num_
{
uint8_t *abyte;
uint16_t *aint;
}num;

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

int char2int(char input)
{
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}

void hex2bin(const char *src, uint8_t *target)
{
  while (*src && src[1])
  {
    *(target++) = char2int(*src) * 16 + char2int(src[1]);
    src += 2;
  }
}

void touchsWith2Byte(){
    // RCVBUFF[2] = 0;
    uint8_t tmpbuff[2] = {0};
    hex2bin(RCVBUFF,tmpbuff);
    uint8_t tmpmid = tmpbuff[0];
    tmpbuff[0] = tmpbuff[1];
    tmpbuff[1] = tmpmid;
    num.abyte = tmpbuff;
    unsigned int pState  = *(num.aint);
    touchObj.set16Pins(pState);
    touchObj.updateData();
    Serial.println("touchsWith2Byte run");
    for(int i =0;i < 128;i++){
      RCVBUFF[i] = 0;
    }
}


void touchPin(int pNumber)
{
  touchObj.setPin(pNumber,false);
  touchObj.updateData();
   unsigned int tmpdat = touchObj.getStates();
   Serial.println(String(tmpdat).c_str());
}
void untouchPin(char pNumber)
{
  touchObj.setPin(pNumber,true);
  touchObj.updateData();
  // unsigned int tmpdat = touchObj.getStates();
  // Serial.println(String(tmpdat).c_str());
}


void touchOnePin(int pNum)
{
  isStart = true;
  touchPin(pNum);
  delay(touchTime);
  untouchPin(pNum);
  delay(delayTime);
  isStart = false;
}

void untouchAll()
{
  touchObj.clearAllTouch();
}
void touchAll()
{
  touchObj.setAllTouch();
}

void touchCmd(char cmd)
{
  if(cmd == '$'){
    delay(1000);
    ESP.restart();  //复位esp32
  }else{
    int touchNum = -1;
    switch(cmd){
      case '1':
      {
        touchNum = 0;
      }
      break;
      case '2':
      {
        touchNum = 1;
      }
      break;
      case '3':
      {
        touchNum = 2;
      }
      break;
      case '4':
      {
        touchNum = 3;
      }
      break;
      case '5':
      {
        touchNum = 4;
      }
      break;
      case '6':
      {
        touchNum = 5;
      }
      break;
      case '7':
      {
        touchNum = 6;
      }
      break;
      case '8':
      {
        touchNum = 7;
      }
      break;
      case '9':
      {
        touchNum = 8;
      }
      break;
      case 'a':
      {
        touchNum = 9;
      }
      break;
      case 'b':
      {
        touchNum = 10;
      }
      break;
      case 'c':
      {
        touchNum = 11;
      }
      break;
      case 'd':
      {
        touchNum = 12;
      }
      break;
      case 'e':
      {
        touchNum = 13;
      }
      break;
      case 'f':
      {
        touchNum = 14;
      }
      break;
      case 'g':
      {
        touchNum = 15;
      }
      break;
      default:
      {
        touchNum = -1;
      }
      break;
    }
    if(isStart){
      return;
    }else{
      if(touchNum != -1)
      {
        touchOnePin(touchNum);
        touchNum = -1;
      }
    }
  }
}
