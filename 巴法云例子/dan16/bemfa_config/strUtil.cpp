
#include "strUtil.h"
#include "webConfigData.h"
#include "APAndSTAMode.h"
/***
s+数字: 控制电机进⾏行行抬笔和落笔，数字的值为笔的坐标
dx+坐标值+y+坐标值:设置原点坐标，把当前的位置设置为坐标系相对原点的x,y坐标值
rc: 设置移动动作为相对当前位置的移动值
ac: 设置移动动作为相对原点的绝对坐标值
x+移动值+y+移动值: 向x⽅方向和y⽅方向移动⼀一定距离，根据rc/ac设置使⽤用相对或绝对坐标 
p+毫秒值: 发送暂停时间，主要为缓冲前⼀一动作⽐比较耗时的情况或其他等待
h:机器回到设备物理原点
f+数字:设置机器移动速度
****/


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

