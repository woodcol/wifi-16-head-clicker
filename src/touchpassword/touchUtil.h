#ifndef __TOUCHUTIL_H__
#define __TOUCHUTIL_H__

/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

//初始化点击头
void touchInit();

//让一个编号的点击头按下,pNumber:值从1~16,表示J1~J16的点击头
void touchPin(int pNumber);
//让一个编号的点击头抬起来,pNumber:值从1~16,表示J1~J16的点击头
void untouchPin(char pNumber);
//让一个编号的占击头先按下再抬起,即完成一个点击动作,pNum:值从1~16,表示J1~J16的点击头
void touchOnePin(int pNum);
//让16个点击头都抬起
void untouchAll();
//让16个点击对都按下
void touchAll();

//同时操作16个点击头
//设置用两个字节的16进制数设置16个点击头的状态
void setState16(unsigned int states);

//按点击头编号设置某个点击头的状态,只改变点击头状态位,但不发送到点击头,pNumber:值从1~16,表示J1~J16的点击头
void setPin(int pNumber,bool state);

//更新设置的点击头状态到点击头
void updateState();

#endif /* INC_TOUCH_H_ */
