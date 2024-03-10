# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import touchDriver as t
import time
from machine import Timer
#实始化一个点击器控制实例对象
tobj = t.TouchObj()
#pNumber按键按下,pNumber从0~15
def touchPin(pNumber):
    tobj.setPin(pNumber,False)
    tobj.updateData()
#pNumber按键抬起,pNumber从0~15
def unTouchPin(pNumber):
    tobj.setPin(pNumber,True)
    tobj.updateData()
#按下并抬起,即点按一次
def touchOncePin(pNumber):
    touchPin(pNumber)
    time.sleep_ms(100)  #按下时间为50毫秒
    unTouchPin(pNumber)
    time.sleep_ms(100)  #抬起后再等50毫秒

#使用16位的按键状态数一次控制所有点击头状态
def setAllPinStates(state):
    tobj.set16Pins(state)
    tobj.updateData()


Timerlist = [2500,2500,2500,2500,2500,2500,2500,2500]

keynum = -1


def touchUP(tim):
    global keynum
    unTouchPin(keynum)
    keynum += 1
    if keynum >= len(Timerlist):
        keynum = 0
    tim.init(period=Timerlist[keynum],mode=Timer.ONE_SHOT ,callback=onTimerEnd)
#定时器时间达到后的回调函数
def onTimerEnd(tim):
    global keynum
    touchPin(keynum)
    tim.init(period=80,mode=Timer.ONE_SHOT ,callback=touchUP)

def runTimer(tim):
    #启动定时器为执行1次,延时时间由keynum所标时间数组值决定
    tim.init(period=Timerlist[keynum],mode=Timer.ONE_SHOT ,callback=onTimerEnd)

nowTouchState = 0 #0:当前停止状态,1:当关前运行状态,2:当前为暂停状态,3:当前为复位重启状态
#短按函数
def shotDown(tim):
    global nowTouchState
    global keynum
    if nowTouchState == 0:      #在停止状态下按了按键 
        keynum = 0              #设置当前运行从头开始
        nowTouchState = 1       #设置当前状态为运行状态
        runTimer(tim)           #启动工作延时定时器  
    elif nowTouchState == 1:    #当前工作状态为正常运行,则状态要改为暂停运行
        tim.deinit()            #关闭定时器,保留当前按键延时数组值keynum不变
        nowTouchState = 2       #设置当前工作状态为暂停状态
    elif nowTouchState == 2:    #当前工作状态为暂停状态,则启动定时器继续工作
        runTimer(tim)
        nowTouchState = 1       #设置为正常工作状态
#长按函数
def longDown(tim):
    global nowTouchState
    global keynum
    if nowTouchState == 0:      #在停止状态下长按了按键 
        keynum = 0              #设置当前运行从头开始
        nowTouchState = 1       #设置当前状态为运行状态
        runTimer(tim)           #启动工作延时定时器  
    elif nowTouchState == 1:    #当前工作状态为正常运行,则状态要改为暂停运行
        tim.deinit()            #关闭定时器,保留当前按键延时数组值keynum不变
        nowTouchState = 2       #设置当前工作状态为暂停状态
    elif nowTouchState == 2:    #当前工作状态为暂停状态,则启动定时器并初始化为从头开始
        keynum = 0              #初始化点击头为重头开始
        nowTouchState = 1       #设置为正常工作状态
        runTimer(tim)

#主函数,点击器程序从这里开始运行
def main():
    setAllPinStates(0b1111111111111111)
    tim = Timer(-1)  #新建一个虚拟定时器
    keydowntime = 0
    keyuptime = 0
    #按键检测循环
    while True:
        tkey = tobj.key.value()
        if not tkey:
            keydowntime = time.time()
        else:
            keyuptime = time.time()
        if keydowntime and keyuptime and keyuptime > keydowntime:
            dtime = keyuptime - keydowntime
            if dtime > 0 and dtime < 2:
                keydowntime = 0
                shotDown(tim)
            elif dtime > 0 and dtime >= 2:
                keydowntime = 0
                longDown(tim)
        else:
            time.sleep_ms(10)

    
if __name__ == '__main__':  
    main()
    