
#ifndef __TASKBUTTON_H__
#define __TASKBUTTON_H__
/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/
#include <Scheduler.h>
#define LED 2
#define Key 2   //板子上的按钮引脚定义

class buttonTask : public Task {
protected:
    void loop();
};

#endif /* INC_TOUCH_H_ */
