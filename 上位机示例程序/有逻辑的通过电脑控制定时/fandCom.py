#!/usr/bin/env python
# -*- coding: utf-8 -*-
#本代码来自所出售产品的淘宝店店主编写
#未经受权不得复制转发
#淘宝店：https://fengmm521.taobao.com/
#再次感谢你购买本店产品
import os,sys
import time
import json
import subprocess

def saveSerialConfig(comnum):
    tmpdic = {"btv":115200,"port":comnum}
    f = open('config.txt','w')
    f.write(json.dumps(tmpdic))
    f.close()

def saveuploadBAT(outstr):
    f = open('upload.bat','w')
    f.write(outstr)
    f.close()

def findComPortFromStr(s):
    tmps = s.split('\n')
    for i,v in enumerate(tmps):
        tmpv = v.replace('\r','')
        if tmpv.find('Serial port COM') != -1:
            ttcom = tmpv.split(' ')[-1]
            return ttcom
    return None
            
def getprojectPath():
    if os.path.exists('config.txt'):
        f = open('config.txt','r')
        tmppth = f.read()
        f.close()
        fpth = tmppth.replace('\n','').replace('\r','').replace(' ','').replace('\t','')
        return fpth
    else:
        return 'comtest'

def getProjectFS():
    if os.path.exists('fconfig.txt'):
        f = open('fconfig.txt','r')
        flist = f.readlines()
        f.close()
        fpths = []
        for i,v in enumerate(flist):
            fpth = v.replace('\n','').replace('\r','').replace(' ','').replace('\t','')
            if len(fpth) > 0:
                fpths.append(fpth)
        return fpths
    return []
def getComPort():
    cmd = 'esptool.py.exe run'
    p=subprocess.Popen(cmd, shell=False, stdout=subprocess.PIPE,stderr=subprocess.STDOUT) # 使用管道
    line=p.stdout.read().decode("utf8")
    print(line)
    comnum = findComPortFromStr(line)
    print(comnum)
    saveSerialConfig(comnum)
    time.sleep(1)
    return
def main():
    getComPort()
if __name__ == '__main__':
    main()