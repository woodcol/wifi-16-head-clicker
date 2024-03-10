#!/usr/bin/env python
# -*- coding: utf-8 -*-
#本代码来自所出售产品的淘宝店店主编写
#未经受权不得复制转发
#淘宝店：https://fengmm521.taobao.com/
#再次感谢你购买本店产品
from ast import Try
import os,sys
import serial
import time
import json

try:
    import ch340usb
except Exception as e:
    pass

from sys import version_info  

isTest = False


#'@'工作模式字典
type2Pins = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}


#获取当前python版本
def pythonVersion():
    return version_info.major


def readcom(t):
    n = t.inWaiting()
    while n<=0:
        time.sleep(0.01)
        n = t.inWaiting()
    pstr = t.read(n)
    if pythonVersion() > 2:
        print(pstr.decode())
    else:
        print(pstr)
    

def sendcmd(t,cmd):
    sendstr = cmd
    # if cmd[-1] != '\r':
    #     sendstr += '\r'
    print(sendstr)
    if pythonVersion() > 2:
        s = t.write(sendstr.encode())
    else:
        s = t.write(sendstr.encode())
    t.flush()

def sendAndread(t,v):
    if isTest:
        f = open('test.txt','a')
        f.write(v + '\n')
        f.close()
    else:
        sendcmd(t,v)
        time.sleep(0.03)
        readcom(t)


def getSerialConfig():
    f = open('config.json','r')
    dat = f.read()
    f.close()
    tmpdict = json.loads(dat)
    return tmpdict

def getPinDat(p):
    return type2Pins[p]

def runcom():
    
    conf = getSerialConfig()
    dev = conf['port']
    btv = conf['btv']
    t = serial.Serial(dev,btv,timeout=1)
    if t:
        print(t.name)               #串口名
        print(t.port)               #串口号
        print(t.baudrate)           #波特率
        print(t.bytesize)           #字节大小
        print(t.parity)             #校验位N－无校验，E－偶校验，O－奇校验
        print(t.stopbits)           #停止位
        print(t.timeout)            #读超时设置
        print(t.writeTimeout)       #写超时
        print(t.xonxoff)            #软件流控
        print(t.rtscts)             #硬件流控
        print(t.dsrdtr)             #硬件流控
        print(t.interCharTimeout)   #字符间隔超时
        print('-'*10)
        # readcom(t)
        for i in range(2):
            print(2-i)
            time.sleep(1)
        sendAndread(t, chr(0x04)) #发送ctrl+d 软重启指令
        time.sleep(1)
        t.close()
    else:
        print('串口不存在')
    

def main():
    runcom()
    timecount = 5
    for i in range(timecount):
        print(timecount-i)
        time.sleep(1)
if __name__ == '__main__':
    main()