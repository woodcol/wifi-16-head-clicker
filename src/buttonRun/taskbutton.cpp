//按钮是否被按下检测线程
#include <ESP8266WiFi.h>
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


extern bool isRun;

    void buttonTask::loop()  {
        delay(50);
        int keystate = digitalRead(Key);
        if(!keystate){ //按键按下一次
            isRun = !isRun;
        }
    }
