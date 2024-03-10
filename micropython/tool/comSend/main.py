#!/usr/bin/env python
# -*- coding: utf-8 -*-
#本代码来自所出售产品的淘宝店店主编写
#未经受权不得复制转发
#淘宝店：https://fengmm521.taobao.com/
#再次感谢你购买本店产品
import os,sys
from machine import UART
import time
import touchutil as tapi


uart = None

def fenliUART():
    global uart
    # uart.write('repl fenli\n')
    os.dupterm(None, 1)

def fujiaRELP():
    global uart
    os.dupterm(uart, 1)
    # uart.write('repl fujia\n')


def readcom(t):
    n = t.inWaiting()
    while n<=0:
        time.sleep(0.01)
        n = t.inWaiting()
    pstr = t.read(n)
    print(pstr.decode())
    return pstr
    

def sendcmd(t,cmd):
    sendstr = cmd
    if cmd[-1] != '\r':
        if type(sendstr) != str:
            sendstr = sendstr.decode() + '\r'
            print(sendstr)
        else:
            sendstr +=  '\r'
    s = t.write(sendstr.encode())
    t.flush()

def sendAndread(t,v):
    sendcmd(t,v)
    time.sleep(0.03)
    return readcom(t)


_configobj = tapi._configobj
    
def setSpeed(pSpeed):
    if pSpeed > 12000:
        print('ERRO,set speed is to fast!')
        return False
    elif pSpeed > 0 and pSpeed <= 12000:
        _configobj.setSpeed(pSpeed)
    return True

#打开串口
def openSerial(btv = 9600):
    global uart
    uart = UART(0, baudrate=btv)
    fenliUART()
    return uart

#移动到绝对坐标xy
def moveTo(x,y):
    cmds = []
    cmds.append(tapi.conventCMD('ac'))
    tmpstr = 'X'+str(x)+'Y'+str(y)
    cmds.append(tapi.conventCMD(tmpstr))
    for i,v in enumerate(cmds):
        if v:
            sendAndread(_configobj.t, v)
        else:
            print('cmd erro:' + v)

#移动到相对当前位置的相对坐标
def moveBy(x,y):
    cmds = []
    cmds.append(tapi.conventCMD('rc'))
    tmpstr = 'X'+str(x)+'Y'+str(y)
    cmds.append(tapi.conventCMD(tmpstr))
    for i,v in enumerate(cmds):
        if v:
            sendAndread(_configobj.t, v)
        else:
            print('cmd erro:' + v)

def delayMS(tms):
    tmpstr = 'p' + str(tms)
    cmd = tapi.conventCMD(tmpstr)
    sendAndread(_configobj.t, cmd)

#设置点击头按下时电机角度
def setTouchDoneValue(v):
    _configobj.tDoneValue = v

def setTouchUpValue(v):
    _configobj.tUpValue = v

def touchDone(p = 'noValue'):
    if p == 'noValue':
        tmpstr = 's' + str(_configobj.tDoneValue)
        cmd = tapi.conventCMD(tmpstr)
        sendAndread(_configobj.t, cmd)
    else:
        tmpstr = 's' + str(p)
        cmd = tapi.conventCMD(tmpstr)
        sendAndread(_configobj.t, cmd)

def touchUp(p = 'noValue'):
    if p == 'noValue':
        tmpstr = 's' + str(_configobj.tUpValue)
        cmd = tapi.conventCMD(tmpstr)
        sendAndread(_configobj.t, cmd)
    else:
        tmpstr = 's' + str(p)
        cmd = tapi.conventCMD(tmpstr)
        sendAndread(_configobj.t, cmd)

def setOP(x = 0,y = 0):
    tmpstr = 'dx' + str(y) + 'y' + str(y)
    cmd = tapi.conventCMD(tmpstr)
    sendAndread(_configobj.t, cmd)

moveSpeed = 8000
jiaoSpeed = 2000

pup = -6
po = 0
pdown = 6
pJiaoY = -40

def moveJiao(x):
    setSpeed(moveSpeed)
    touchUp(pup)
    time.sleep(0.3)
    moveTo(x, pJiaoY)
    time.sleep(0.1)
    moveTo(0, pJiaoY)
    setSpeed(jiaoSpeed)
    time.sleep(0.3)
    touchDone(pdown)
    time.sleep(0.3)
    touchUp(pup)
    delayMS(0.5)
    time.sleep(0.1)
    setSpeed(moveSpeed)

def dianjiao(x,y):
    setSpeed(moveSpeed)
    moveTo(x, pJiaoY)
    time.sleep(0.1)
    moveTo(x, y)
    delayMS(0.1)
    time.sleep(1)
    setSpeed(jiaoSpeed)
    touchDone(pdown)
    time.sleep(0.3)
    touchUp(0)
    time.sleep(0.3)
    delayMS(0.1)
    moveJiao(x)

def runloop(sx = -1,sy = -1):
    btv = 115200
    t = openSerial(btv)
    t.write("uart start...\n")
    # print(t.name)               #串口名
    # print(t.port)               #串口号
    # print(t.baudrate)           #波特率
    # print(t.bytesize)           #字节大小
    # print(t.parity)             #校验位N－无校验，E－偶校验，O－奇校验
    # print(t.stopbits)           #停止位
    # print(t.timeout)            #读超时设置
    # print(t.writeTimeout)       #写超时
    # print(t.xonxoff)            #软件流控
    # print(t.rtscts)             #硬件流控
    # print(t.dsrdtr)             #硬件流控
    # print(t.interCharTimeout)   #字符间隔超时
    # print('-'*10)
    time.sleep(5)
    readcom(t)
    setOP() #设置原点为当前坐标
    
    setSpeed(3000)
    touchUp(pup)
    time.sleep(0.3)
    moveTo(0, pJiaoY)
    time.sleep(0.1)
    touchDone(pdown)
    time.sleep(0.1)
    touchUp(pup)
    delayMS(0.1)
    setTouchDoneValue(pdown)
    setTouchUpValue(po)
    xn = 15
    yn = 15
    l = 12
    if True:
        for ix in range(10):
            time.sleep(1)
            touchDone()
            time.sleep(0.3)
            touchUp()
            time.sleep(0.3)
    else:
        for ix in range(xn):
            if ix < sx:
                continue
            for iy in range(yn):
                if ix == sx and iy <= sy:
                    continue
                dianjiao(ix*l, iy*l)
        moveTo(0, 0)
        time.sleep(30)
    fujiaRELP()
def main():
    try:
        runloop()
    except Exception as e:
        uart.write("runloop erro:%s\n"%(str(e)))
        pass
    fujiaRELP()
if __name__ == '__main__':
    main()