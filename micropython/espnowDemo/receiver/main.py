#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2022-11-01 14:06:58
# @Author  : Your Name (you@example.org)
# @Link    : http://example.org
# @Version : $Id$

import time
import network
import espnow
import touchDriver as t
from machine import Timer
import random

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

allUntouch = 0b1111111111111111    #所有点击头抬起
danshu     = 0b1010101010101010    #单数按下
shuanshu   = 0b0101010101010101    #双数按下

startFunc = None

dTime  = None          #点击头按下随机时间
dTime1 = None      #点击头单数按下完成后延时时间
dTime2 = None        #点击头双数按下后的延时   

def randomTime(tmin,t2max):
    dtmp = randint(tmin,t2max)      #取随机数1200-1500之间
    return dtmp 

def delay2(tim):
    global startFunc
    dt2 = randomTime(dTime2[0],dTime2[1])
    tim.init(period=dt2,mode=Timer.ONE_SHOT ,callback=startFunc)  #进行接下来的循环

def touchUP2(tim):
    setAllPinStates(allUntouch)   #抬起所有点击头
    dt = randomTime(dTime[0],dTime[1])
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=delay1)

def touchDown2(tim):
    setAllPinStates(shuanshu)     #双数按下
    dt = randomTime(dTime[0],dTime[1])
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP2)

def delay1(tim):
    dt1 = randomTime(dTime1[0],dTime1[1])
    tim.init(period=dt1,mode=Timer.ONE_SHOT ,callback=touchDown2)

def touchUP1_2(tim):
    setAllPinStates(allUntouch)  #抬起所有点击头
    dt = randomTime(dTime[0],dTime[1])
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=delay1)

def touchDown1_2(tim):
    setAllPinStates(danshu)     #单数按下
    dt = randomTime(dTime[0],dTime[1])
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP1_2)

def touchUP1_1(tim):
    setAllPinStates(allUntouch) #抬起所有点击头
    dt = randomTime(dTime[0],dTime[1])
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchDown1_2)

def touchDown1_1(tim):
    setAllPinStates(danshu)     #单数按下
    dt = randomTime(dTime[0],dTime[1])
    tim.init(period=dt,mode=Timer.ONE_SHOT ,callback=touchUP1_1)

def runTimer(tim):
    touchDown1_1(tim)

def run(tim):
    global startFunc
    startFunc = runTimer
    runTimer(tim)

def stop(tim):
    tim.deinit()                #关闭定时器
    setAllPinStates(allUntouch) #抬起所有点击头


def main():
    global dTime,dTime1,dTime2
    dTime     = (40,50)          #点击头按下随机时间
    dTime1    = (2000,2200)      #点击头单数按下完成后延时时间
    dTime2    = (250,300)        #点击头双数按下后的延时  

    tim = Timer(-1)  #新建一个虚拟定时器 
    # A WLAN interface must be active to send()/recv()
    sta = network.WLAN(network.STA_IF)
    sta.active(True)
    sta.disconnect()   # Because ESP8266 auto-connects to last Access Point

    e = espnow.ESPNow()
    e.active(True)

    peer = b'\xd8\xbf\xc0\xff\x54\x30'   #这里是控制主机的Mac地址
    print(str(peer))
    e.add_peer(peer)
    while True:
        time.sleep(1)
        print('sleep 1')
        while True:
            host, msg = e.recv()
            if msg:  #如果接收到消息
                print(host, msg)
                if msg == b'1':    #接收到开启指令
                    run(tim)
                    e.send(peer, b'1')
                elif msg == b'0':  #接收到暂停指令
                    stop(tim)
                    e.send(peer, b'0')
if __name__ == '__main__':
    main()
