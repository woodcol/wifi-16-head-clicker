#include "touch.h"
#include "webConfigData.h"

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/


// esp8266_arduino_wemos pins
//#define D0 16
//#define D1 5 // I2C Bus SCL (clock)
//#define D2 4 // I2C Bus SDA (data)
//#define D3 0
//#define D4 2 // Same as "LED_BUILTIN", but inverted logic
//#define D5 14 // SPI Bus SCK (clock)
//#define D6 12 // SPI Bus MISO 
//#define D7 13 // SPI Bus MOSI
//#define D8 15 // SPI Bus SS (CS)
//#define D9 3 // RX0 (Serial console)
//#define D10 1 // TX0 (Serial console)

//因为接入74hc595的芯片使用5V,所以在接入前使用了一个三极管进行3.3V到5V电平转换，下边因接有三极管反向，所以与实际芯片控制端信号相反
#define HC595_OE_  16  //GPIO16,HC595的输出使能三态门控制端，低电平为开，高电平为关
#define HC595_RCK_ 14  //GPIO14,HC595的输入数据锁存到输出的控制脉冲，控制为下降沿触发数据锁存(芯片为上升沿，但有三极管反相)
#define HC595_SRCK_ 13 //GPIO13,HC595的输入数据移位动作控制脉冲，控制为下降沿动作(芯片为上升沿，但有三极管反相)
#define HC595_SER_ 12  //GPIO12,HC595的移位输入数据，在SRCK有控制脉冲时，这里的数据会被移入寄存器(实际移入数据这此数据相反，但有三极管反相)

#define senddelay 10
#define mdelay 1

void sendStep(bool dat)
{
  digitalWrite(HC595_SER_,!dat);
  delayMicroseconds(senddelay);
  digitalWrite(HC595_SRCK_,HIGH);
  delayMicroseconds(senddelay);
  digitalWrite(HC595_SRCK_,LOW);
  delayMicroseconds(senddelay);
  digitalWrite(HC595_SRCK_,HIGH);
  delayMicroseconds(senddelay);
  digitalWrite(HC595_SER_,false);
}

void updateOUT()
{
  digitalWrite(HC595_RCK_,HIGH);
  delayMicroseconds(senddelay);
  digitalWrite(HC595_RCK_,LOW);
  delayMicroseconds(senddelay);
  digitalWrite(HC595_RCK_,HIGH);
  delayMicroseconds(senddelay);
}

HC595Touch::HC595Touch(void):m_data(0x0ffff)
{
    
}

void HC595Touch::initObj()
{
    pinMode(HC595_OE_, OUTPUT);
    pinMode(HC595_RCK_, OUTPUT);
    pinMode(HC595_SRCK_, OUTPUT);
    pinMode(HC595_SER_, OUTPUT);
    delayMicroseconds(senddelay);
    digitalWrite(HC595_SER_,LOW);
    delayMicroseconds(senddelay);
    digitalWrite(HC595_RCK_,HIGH);
    delayMicroseconds(senddelay);
    digitalWrite(HC595_SRCK_,HIGH);
    delayMicroseconds(senddelay);
    digitalWrite(HC595_OE_,HIGH);
    delayMicroseconds(senddelay);
    isTestTrue = true;
}

void HC595Touch::initTouch()         //初始化所有引脚为非点击
{
  initObj();
  clearAllTouch();
}

void HC595Touch::setPin(int pNumber,bool state)
{
  if(state){
    m_data = m_data | (0x001<<pNumber);
  }else{
    m_data = m_data & (~(0x001<<pNumber));
  }
}

void HC595Touch::updateData()        //更新数据到hc595的输出
{
  unsigned int tmpdat = m_data;
  for(int i=0;i<16;i++){
    bool st = (tmpdat<<i)&0x08000;
    sendStep(st);
  }
  updateOUT();
  
}

void HC595Touch::setFront8Pins(char states)       //设置前8个引脚状态,即设置J0,j1,j2,j3,j4,j5,j6,j7的状态
{

    m_data = m_data & (states&0x0ff00);
}
void HC595Touch::setLast8Pins(char states)        //设置后8个引脚状态,即设置J8,J9,Ja,Jb,Jc,Je,Je,Jf的状态
{
    m_data = m_data & ((states&0x0ff00)<<8);
}
void HC595Touch::set16Pins(unsigned int states)   //设置所有引脚状态
{
  m_data = states;
}
void HC595Touch::clearAllTouch()                   //设置所有引脚为非点击
{
  m_data = 0x0ffff;
  updateData();
}
void HC595Touch::setAllTouch()                     //设置16个引脚为点击
{
  m_data = 0x0;
  updateData();
}

//16个点击头每接收到一个test则统一向后移一个动作
bool isDelayM = false;
void HC595Touch::test()                     //设置16个引脚为点击
{
//  m_data = 0x0;
//  updateData();
  if(isTestTrue){
    digitalWrite(HC595_SER_,false);
    isTestTrue = false;
  }else{
    digitalWrite(HC595_SER_,true);
    isTestTrue = true;
  }
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }
  
  digitalWrite(HC595_SRCK_,HIGH);
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }

  digitalWrite(HC595_SRCK_,LOW);
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }
  digitalWrite(HC595_SRCK_,HIGH);
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }
//  digitalWrite(HC595_SER_,false);
//  delayMicroseconds(senddelay);
  digitalWrite(HC595_RCK_,HIGH);
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }
  digitalWrite(HC595_RCK_,LOW);
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }
  digitalWrite(HC595_RCK_,HIGH);
  if(isDelayM){
    delay(mdelay);
  }else{
    delayMicroseconds(senddelay);
  }
  
}
unsigned int HC595Touch::getStates()
{
  return m_data;
}
