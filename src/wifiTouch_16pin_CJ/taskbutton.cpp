//按钮是否被按下检测线程
#include "include.h"
#include "taskbutton.h"
/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

extern bool isNetOK;

int delayCount = 30;  //定义，如果按钮被常按3秒以上，则运行smartconfig重新设置wifi模块
int delayCountSerial = 80; //定义,如果按钮被常按6秒以上时,则不再使用wifi而是使用串口工作模式
int delayCount_now = 1;

    void buttonTask::loop()  {
        delay(100);
        if(isNetOK){
            int keystate = digitalRead(Key);
            if(!keystate){
              delayCount--;
              delayCountSerial--;
            }else{
              if(delayCount <= 0 && delayCountSerial > 0){
                Serial.println("key1");
                isNetOK = false;
                delayCount = 30;
                delayCountSerial = 100;
                WiFi.disconnect(false); //断开连接并清除保存的wifi
                setWifiType('0');
                untouchAll();
                smartConfig();
              }else if(delayCountSerial <= 0){
                Serial.println("key2");
                delayCount = 30;
                delayCountSerial = 100;
                WiFi.disconnect(false); //断开连接并清除保存的wifi
                setWifiType('2');       //设置为使用串口控制命令
                untouchAll();
              }
            }
            
          }else{
            int keystate = digitalRead(Key);
            if(!keystate){
              delayCount_now--;
            }
            if(delayCount_now <= 0){
              
              if(isSetSerial()){
                Serial.println("key3");
                changeToNet();
              }else{
                Serial.println("key4");
                delayCountSerial = 100;
                isNetOK = false;
                setWifiType('2');
                untouchAll();
                
              }
                
            }
          }
          
    }