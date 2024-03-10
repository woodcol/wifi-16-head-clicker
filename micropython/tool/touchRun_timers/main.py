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


nowTouchState = 0 #0:当前停止状态,1:当关前运行状态,2:当前为暂停状态,3:当前为复位重启状态

allUntouch = 0b1111111111111111    #所有点击头抬起
danshu     = 0b1010101010101010    #单数按下
shuanshu   = 0b0101010101010101    #双数按下
Alltouch   = 0    #双数按下

def stopAll():
    global isTimerRun
    global TIMEOBJ
    global nowTouchState
    if isTimerRun:
        for i,v in enumerate(TIMEOBJ):
            v.deinit()          #循环次数到了，程序结束
        isTimerRun = False
    nowTouchState = 0
    setAllPinStates(0b1111111111111111)
    print('stop work!')

baseSecend = 15

#定时器时间达到后的回调函数
def onTimerEnd(tim):
    global isTimerRun
    isTimerRun = False
    print('start logic')
    thetim = 0
    for i,v in enumerate(TIMEOBJ):
        if tim == v:
            if i == 0:
                print('touch 15')
                setAllPinStates(0b0000111111111111)
                time.sleep_ms(100)
                setAllPinStates(allUntouch)
            elif i == 1:
                print('touch 30')
                setAllPinStates(0b1111000011111111)
                time.sleep_ms(100)
                setAllPinStates(allUntouch)
            elif i == 2:
                print('touch 45')
                setAllPinStates(0b1111111100001111)
                time.sleep_ms(100)
                setAllPinStates(allUntouch)
            elif i == 3:
                print('touch 60')
                setAllPinStates(0b1111111111110000)
                time.sleep_ms(100)
                setAllPinStates(allUntouch)
            isTimerRun = True
            tim.init(period=baseSecend*1000*(i+1),mode=Timer.ONE_SHOT ,callback=onTimerEnd)
    


#短按按键函数
def shotDown(tims):
    global nowTouchState
    global isTimerRun
    print('button push!')
    if nowTouchState == 0:      #在停止状态下按了按键 
        nowTouchState = 1       #设置当前状态为运行状态
        if not isTimerRun:
            print('start work:')
            for i,v in enumerate(tims):
                onTimerEnd(v)           #启动工作延时定时器
    elif nowTouchState == 1:    #当前工作状态为正常运行,则状态要改为暂停运行
        stopAll()

#主函数,点击器程序从这里开始运行
def main():
    global TIMEOBJ,timDict
    setAllPinStates(0b1111111111111111)
    tims = [Timer(-1),Timer(-2),Timer(-3),Timer(-4)]  #新建一个虚拟定时器
    TIMEOBJ = tims
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
                shotDown(tims)
        else:
            time.sleep_ms(10)

    
if __name__ == '__main__':  
    main()
    