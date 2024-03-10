#ifndef __INCLUDE_H__
#define __INCLUDE_H__

/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

#define LED 2
#define Key 2   //板子上的按钮引脚定义

#define EEPROMSIZE 64    //申请的EEPROM大小字节数

#define MAX_SRV_CLIENTS 4   //wifi服务器的最大允许连接数量，这里设置为只允许一台客户端进行连接

#include "dataUtil.h"
#include "netUtil.h"
#include "taskbutton.h"
#include "taskTouch.h"
#include "touchUtil.h"


#endif /* INC_TOUCH_H_ */
