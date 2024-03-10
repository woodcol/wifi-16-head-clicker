#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2022-11-01 14:06:58
# @Author  : Your Name (you@example.org)
# @Link    : http://example.org
# @Version : $Id$

import time
import os
import touchDriver as t
from machine import Timer
import urandom

runcount = 10
dtimeX = 3 *60*1000
isStop = 0        #如果这值是1，就是25次后会停，如果是0就是25次后等一个时间dtimeX

#实始化一个点击器控制实例对象
tobj = t.TouchObj()

def randint(min, max):
    span = max - min + 1
    div = 0x3fffffff // span
    offset = urandom.getrandbits(30) // div
    val = min + offset
    # val =  random.randint(min, max)
    return val

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

allUntouch = 0b1111111111111111    #所有点击头抬起
danshu     = 0b1010101010101010    #单数按下
shuanshu   = 0b0101010101010101    #双数按下

startFunc = None

f = open('config.txt','r')
fstr = f.read()
f.close()

dtmp = fstr.replace(' ','').replace('\t','').replace('\r','').replace('|',',')
lines = dtmp.split('\n')
for i,v in enumerate(lines):
    if len(v) > 1:
        lines[i] = v.split('#')[0]
        lines[i] = lines[i].split(',')

dTime  =   (int(lines[0][0]),int(lines[0][1]))   
dTime_12 = (int(lines[0][0]),int(lines[0][1]))
if len(lines[0]) == 4:
    dTime_12 = (int(lines[0][2]),int(lines[0][3]))   
dTime1 =   (int(lines[1][0]),int(lines[1][1])) 
dTime2 =   (int(lines[2][0]),int(lines[2][1]))    
dTime2_1 = None

if len(lines) == 4:
    dTime2_1 = (int(lines[2][0]),int(lines[2][1])) 
    dTime2 =   (int(lines[3][0]),int(lines[3][1]))     

 
isLog = True

runtimes = runcount

tim = Timer(-1)  #新建一个虚拟定时器 
    # print(dTime,dTime1,dTime2)
def showLog(s,d = None):
    time.sleep_ms(1)
    if isLog:
        print(s,d)

def delay2(t):
    global startFunc
    global dTime2,tim
    dt = randint(dTime2[0],dTime2[1])
    showLog('delay2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=startFunc)  #进行接下来的循环


def touchUP2_2(t):
    global dTime,tim
    setAllPinStates(allUntouch)   #抬起所有点击头
    dt = randint(dTime[0],dTime[1])
    showLog('touchUP2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=delay2)

def touchDown2_2(t):
    global dTime,tim
    setAllPinStates(shuanshu)     #双数按下
    dt = randint(dTime[0],dTime[1])
    showLog('touchDown2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP2_2)

def delay2_2(t):
    global dTime2_1,tim
    dt = randint(dTime2_1[0],dTime2_1[1])
    showLog('delay2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchDown2_2)  #进行接下来的循环


def touchUP2(t):
    global dTime,tim
    setAllPinStates(allUntouch)   #抬起所有点击头
    dt = randint(dTime[0],dTime[1])
    showLog('touchUP2',dt)
    if dTime2_1:
        tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=delay2_2)
    else:
        tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=delay2)
    

def touchDown2(t):
    global dTime,tim
    setAllPinStates(shuanshu)     #双数按下
    dt = randint(dTime[0],dTime[1])
    showLog('touchDown2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP2)

def delay1(t):
    global dTime1,tim
    dt = randint(dTime1[0],dTime1[1])
    showLog('delay1',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchDown2)

def touchUP1_2(t):
    global dTime,tim
    setAllPinStates(allUntouch)  #抬起所有点击头
    dt = randint(dTime[0],dTime[1])
    showLog('touchUP1_2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=delay1)

def touchDown1_2(t):
    global dTime,tim
    setAllPinStates(danshu)     #单数按下
    dt = randint(dTime[0],dTime[1])
    showLog('touchDown1_2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP1_2)

def touchUP1_1(t):
    global dTime,tim
    setAllPinStates(allUntouch) #抬起所有点击头
    dt = randint(dTime_12[0],dTime_12[1])
    showLog('touchUP1_1',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchDown1_2)


def delay_2min(t):
    global startFunc
    global dTime2,tim
    dt = dtimeX
    showLog('delay2',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=startFunc)  #进行接下来的循环

def touchDown1_1(t):
    global dTime,tim,runtimes
    if runtimes < 0:
        runtimes = runcount
        if isStop == 1:
            stop()
        elif isStop == 0:
            tim.init(period=20,mode=Timer.ONE_SHOT ,callback=delay_2min)
        return 
    else:
        runtimes -= 1
    setAllPinStates(danshu)     #单数按下
    dt = randint(dTime[0],dTime[1])
    showLog('touchDown1_1',dt)
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP1_1)

def runTimer():
    touchDown1_1(None)

isRuning = False
isrun = False

def run():
    global isRuning
    if isRuning:
        return 
    isRuning = True
    global startFunc
    startFunc = touchDown1_1
    runTimer()

def stop():
    global tim,isRuning,isrun,runtimes
    isRuning = False
    isrun = False
    runtimes = runcount
    tim.deinit()                #关闭定时器
    setAllPinStates(allUntouch) #抬起所有点击头

def main():
    global isrun
    setAllPinStates(allUntouch)
    time.sleep_ms(100)
    isrun = False                     #按键状态，是否启动
    while True:  #无限循环
        tkey = tobj.key.value()
        if not tkey:
            print('key down')
            isrun = not isrun
            time.sleep_ms(1000)
            if not isrun:
                stop()
                time.sleep_ms(1000)
        if isrun:
            run()
            time.sleep_ms(100)
        else:
            time.sleep_ms(100)
            
if __name__ == '__main__':
    main()
