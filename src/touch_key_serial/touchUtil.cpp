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

void touchInit()
{
  touchObj.initTouch();
}

void touchPin(int pNumber)
{
  if(pNumber < 1 && pNumber > 16){//点击头只有16个,其他数值不做处理
    return;
  }
  touchObj.setPin(pNumber-1,false);
  touchObj.updateData();
}

void untouchPin(char pNumber)
{
  if(pNumber < 1 && pNumber > 16){//点击头只有16个,其他数值不做处理
    return;
  }
  touchObj.setPin(pNumber-1,true);
  touchObj.updateData();
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

//同时操作16个点击头
void setState16(unsigned int states)
{
  touchObj.set16Pins(states); //0b1111111111111110
}
void setPin(int pNumber,bool state)
{
  if(pNumber < 1 && pNumber > 16){//点击头只有16个,其他数值不做处理
    return;
  }
  touchObj.setPin(pNumber-1,state);
}
void updateState()
{
  touchObj.updateData();
}
