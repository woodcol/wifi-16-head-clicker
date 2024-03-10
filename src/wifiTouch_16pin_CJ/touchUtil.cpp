#include "include.h"
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

int touchNum = -1;

int touchTime  = 80;    //点击屏的时间,即模仿手指点击时在手屏上的停留时间，单位：毫秒

int delayTime  = 50;    //点击后停止时间，单位：毫秒

HC595Touch touchObj = HC595Touch();

extern char RCVBUFF[];

extern uint8_t EEPROMData[];

union num_
{
uint8_t *abyte;
uint16_t *aint;
}num;
void getTouchTIme(){
    uint8_t tmpbuff[2] = {0};
    hex2bin(RCVBUFF,tmpbuff);
    uint8_t tmpmid = tmpbuff[0];
    tmpbuff[0] = tmpbuff[1];
    tmpbuff[1] = tmpmid;
    num.abyte = tmpbuff;
    touchTime = *(num.aint);
    Serial.println("touchTime:");
    Serial.println(tmpbuff[0]);
    Serial.println(tmpbuff[1]);
    Serial.println(touchTime);
    for(int i =0;i < 128;i++){
      RCVBUFF[i] = 0;
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
    for(int i =0;i < 128;i++){
      RCVBUFF[i] = 0;
    }
}


void touchPin(int pNumber)
{
  touchObj.setPin(pNumber,false);
  touchObj.updateData();
  // unsigned int tmpdat = touchObj.getStates();
  // Serial.println(String(tmpdat).c_str());
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
  if(cmd == '~' || cmd == '!' || cmd == '@'){
    switch (cmd) {
        case '~':{
          //复位设置值
          uint8_t datatmp[EEPROMSIZE] = {'1','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z',0,50,0,50,'0','0','0','0','0','0','1','0'};
          for(int i=0;i<EEPROMSIZE;i++){
            EEPROMData[i] = datatmp[i];
          }
          saveDataToEEPROM();
          touchTime = 50;
          untouchAll();
        }
          break;
        case '!':
          // do something
          {
            setTouchType('1');
          }
          break;
        case '@':
          // do something
          {
            setTouchType('2');
          }
          break;
        default:
          // do something
          break;
    }
    return;

  }else if(cmd == '$'){
    //复位wifi
    WiFi.disconnect(false); //断开连接并清除保存的wifi
    setWifiType('0');
    untouchAll();
    smartConfig();
    
  }else{  
      if(EEPROMData[0] == '1'){
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
      }else if(EEPROMData[0] == '2'){
          switch (cmd) {
            //0
                  case '0':
                    touchPin(0);
                    break;
                  case '1':
                    untouchPin(0);
                    break;
                    
            //1
                  case '2':
                    touchPin(1);
                    break;
                  case '3':
                    untouchPin(1);
                    break;
                    
            //2
                  case '4':
                    touchPin(2);
                    break;
                  case '5':
                    untouchPin(2);
                    break;
                    
            //3
                  case '6':
                    touchPin(3);
                    break;
                  case '7':
                    untouchPin(3);
                    break;
                    
            //4
                  case '8':
                    touchPin(4);
                    break;
                  case '9':
                    untouchPin(4);
                    break;
                    
            //5
                  case 'a':
                    touchPin(5);
                    break;
                  case 'b':
                    untouchPin(5);
                    break;
                    
            //6
                  case 'c':
                    touchPin(6);
                    break;
                  case 'd':
                    untouchPin(6);
                    break;
                    
            //7
                  case 'e':
                    touchPin(7);
                    break;
                  case 'f':
                    untouchPin(7);
                    break;
                    
            //8
                  case 'g':
                    touchPin(8);
                    break;
                  case 'h':
                    untouchPin(8);
                    break;
                    
            //9
                  case 'i':
                    touchPin(9);
                    break;
                  case 'j':
                    untouchPin(9);
                    break;
                    
            //10
                  case 'k':
                    touchPin(10);
                    break;
                  case 'l':
                    untouchPin(10);
                    break;
                    
            //11
                  case 'm':
                    touchPin(11);
                    break;
                  case 'n':
                    untouchPin(11);
                    break;
                    
            //12
                  case 'o':
                    touchPin(12);
                    break;
                  case 'p':
                    untouchPin(12);
                    break;
                    
            //13
                  case 'q':
                    touchPin(13);
                    break;
                  case 'r':
                    untouchPin(13);
                    break;
                    
            //14
                  case 's':
                    touchPin(14);
                    break;
                  case 't':
                    untouchPin(14);
                    break;
                    
            //15
                  case 'u':
                    touchPin(15);
                    break;
                  case 'v':
                    untouchPin(15);
                    break;
            
            //all touch
                  case 'x':
                   touchAll();
                    break;
            
            //all untouch
                  case 'y':
                    untouchAll();
                    break;
            //16个点击头每接收到一个test则统一向后移一个动作
                  case 'z':
                    touchObj.test();
                    break;
                  case '#':
                    
                    break;
                  default:
                    // do something
                    Serial.print(String(cmd).c_str());
                    break;
            
              }
      }
  }

  
}
