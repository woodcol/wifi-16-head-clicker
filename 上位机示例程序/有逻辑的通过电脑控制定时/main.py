#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author:洪卫
 

import os,sys
import serial
import time
import json
import timetool
from sys import version_info  

def pythonVersion():
    return version_info.major

key1 = 1
key2 = 2

dtime = 0.040
tcount = 5
tdelay = 0.1



def getTime():
    global dtime,tdelay,tcount
    f = open('timer.txt','r')
    dat = f.read()
    f.close()
    configs = json.loads(dat)
    ttime = int(timetool.conventTimeFromStrConfig(configs['time'])*1000)
    dtime = float(configs['tdtime']/1000.0)
    tcount = configs['tCount']
    tdelay = float(configs['delay']/1000.0)
    print('time1:%s'%(configs['time']))
    print(ttime,dtime,tcount)
    return ttime


dtime2  = 0.040
tcount2 = 5
tdelay2 = 0.1


def getTime2():
    global dtime2,tdelay2,tcount2
    f = open('timer2.txt','r')
    dat = f.read()
    f.close()
    configs = json.loads(dat)
    ttime = int(timetool.conventTimeFromStrConfig(configs['time'])*1000)
    dtime2 = float(configs['tdtime']/1000.0)
    tcount2 = configs['tCount']
    tdelay2 = float(configs['delay']/1000.0)
    print('time:%s'%(configs['time']))
    print(ttime,dtime,tcount)
    return ttime


serialT = None

def readPort():
    global serialT
    time.sleep(0.003)
    n = serialT.inWaiting()
    # print(n)
    if n > 0:
        pstr = serialT.read(n)
        if pythonVersion() == 2:
            # return pstr
            print('setial read:%s'%(pstr))
        else:
            # return str(pstr)
            print('setial read:%s'%(str(pstr)))


def sendCmd(pcmd):
    global serialT
    back = None
    if pythonVersion() > 2:
        back = serialT.write(pcmd.encode())
    else:
        back = serialT.write(pcmd)
    serialT.flush()
    print('send cmd,:%s,isOk:%s'%(pcmd,str(back)))
    return back

#'@'工作模式字典
type2Pins = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}
# type2Pins = {1:['<0000>','<FFFF>'],2:['<0000>','<FFFF>']}


def delaylog(n):
    for i in range(n):
        print('after %ds well exit.'%(n-i))

def touchPin(n):
    sendCmd(type2Pins[n][0])
    time.sleep(dtime)
    sendCmd(type2Pins[n][1])
    time.sleep(tdelay)

def main():
    global serialT,tdelay,tcount
    f = open('config.txt','r')
    jstr = f.read()
    f.close()
    jdic = json.loads(jstr)
    t = serial.Serial(jdic['port'],jdic['btv'],timeout=0.5)
    serialT = t
    time.sleep(2)
    sendCmd('@')
    readPort()

    sendCmd('<FFFF>')

    ttime = getTime()
    tttime2 = getTime2()
    if not (key1 and key2 and ttime):
        print('init timer erro,and exit...')
        return
    isRuned = False
    isRuned2 = False
    while True:
        time.sleep(0.001) #进行一个延时,防止占用CPU过高
        nowtime = int(time.time()*1000)
        if ttime <= nowtime or tttime2 <= nowtime:
            if not isRuned and ttime <= nowtime:
                for i in range(tcount):
                    touchPin(1)          #J1按一下
                    time.sleep(0.9)      #延时等900毫秒
                    touchPin(2)          #J2按一下
                    time.sleep(3)        #延时等3秒，即3000毫秒
                    touchPin(3)          #J3按一下
                isRuned = True
            if not isRuned2 and tttime2 <= nowtime:
                for i in range(tcount2):
                    sendCmd(type2Pins[key2][0])
                    time.sleep(dtime2)
                    sendCmd(type2Pins[key2][1])
                    time.sleep(tdelay2)
                isRuned2 = True
        elif isRuned and isRuned2: #定时器已经运行
            print('run end ...')
            delaylog(10)
            
    
if __name__ == '__main__':
    main()
