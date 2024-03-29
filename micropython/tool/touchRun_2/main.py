# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import touchDriver as t
import time
from machine import Timer
#实始化一个点击器控制实例对象
tobj = t.TouchObj()

TIMEOBJ = None

isTimerRun = False

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

#设置参数
keynum = 0     #下一个要按下的按键号，停止时会设置为0号即J1
delayTIME = 500   #间隔时间，单位为毫秒



def stopAll():
    global isTimerRun
    global TIMEOBJ
    global runtimes
    global keynum
    global nowTouchState
    if isTimerRun:
        TIMEOBJ.deinit()          #循环次数到了，程序结束
        isTimerRun = False
    keynum = 0
    nowTouchState = 0
    setAllPinStates(0b1111111111111111)
    print('stop work!')

#定时器时间达到后的回调函数
def onTimerEnd(tim):
    global keynum
    global nowTouchState
    global isTimerRun
    isTimerRun = False
    print('on touch:J%d'%(keynum+1))
    touchPin(keynum)
    time.sleep_ms(100)
    unTouchPin(keynum)
    time.sleep_ms(100)
    keynum += 1
    if keynum >= 16:
        keynum = 0;
    isTimerRun = True
    tim.init(period=delayTIME,mode=Timer.ONE_SHOT ,callback=onTimerEnd)
    

nowTouchState = 0 #0:当前停止状态,1:当关前运行状态,2:当前为暂停状态,3:当前为复位重启状态
#短按按键函数
def shotDown(tim):
    global nowTouchState
    global keynum
    global isTimerRun
    print('button push!')
    if nowTouchState == 0:      #在停止状态下按了按键 
        keynum = 0              #设置当前运行从头开始
        nowTouchState = 1       #设置当前状态为运行状态
        touchPin(15)
        if not isTimerRun:
            print('start work:')
            onTimerEnd(tim)           #启动工作延时定时器
            
    elif nowTouchState == 1:    #当前工作状态为正常运行,则状态要改为暂停运行
        stopAll()


#主函数,点击器程序从这里开始运行
def main():
    global TIMEOBJ
    setAllPinStates(0b1111111111111111)
    tim = Timer(-1)  #新建一个虚拟定时器
    TIMEOBJ = tim
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
            if dtime > 0:
                keydowntime = 0
                shotDown(tim)
        else:
            time.sleep_ms(10)

    
if __name__ == '__main__':  
    main()
    