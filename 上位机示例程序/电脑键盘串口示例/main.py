#!/usr/bin/python
# -*- coding: utf-8 -*-
#创建SocketServerTCP服务器：
import os
import socket
import time
import platform
from sys import exit
import serial

from sys import version_info

sysSystem = platform.system()

#设置pygame
if sysSystem == 'Windows':  #windows系统
    try:
        import pygame
    except Exception as e:
        print('pygame库安装有问题,重新设置windows系统下的pygame库')
        print('从本地安装pygame库...')
        cmd = 'python -mpip install pygame-2.0.1-cp37-cp37m-win_amd64.whl'
        os.system(cmd)
        time.sleep(1)
        import pygame
elif sysSystem == 'Darwin': #苹果mac os系统
    try:
        import pygame
    except Exception as e:
        print('pygame库安装有问题,重新设置Mac系统下的pygame库')
        print('从本地安装pygame库...')
        cmd = 'python -mpip install pygame-2.0.1-cp37-cp37m-macosx_10_9_intel.whl'
        os.system(cmd)
        time.sleep(1)
        import pygame
elif sysSystem == 'Linux':
    try:
        import pygame
    except Exception as e:
        print('程序没有带linux的pygame库')
        print('从网络安装pygame库')
        cmd = 'python -mpip install pygame'
        os.system(cmd)
        time.sleep(1)
        import pygame

from pygame.locals import *


#获取当前python版本
def pythonVersion():
    return version_info.major


t = None

def readcom():
    global t
    n = t.inWaiting()
    while n<=0:
        time.sleep(0.01)
        n = t.inWaiting()
    pstr = t.read(n)
    if pythonVersion() > 2:
        print(pstr.decode())
    else:
        print(pstr)
    
def sendcmd(cmd):
    global t
    sendstr = cmd
    print(sendstr)
    if pythonVersion() > 2:
        s = t.write(sendstr.encode())
    else:
        s = t.write(sendstr.encode())
    t.flush()

def send(v):
    global t
    sendcmd(v)
    time.sleep(0.03)
    readcom()


BG_IMAGE = 'bg.jpg'
display_width = 500
display_height = 500
BG_Clolor = (0,0,0)
Text_Clolor = (235,190,11)
Main_screen = None

#pygame 显示文字
def text_objects(text, font,isClean = False):
    Color = Text_Clolor
    if isClean:
        Color = BG_Clolor
    textSurface = font.render(text, True, Color)
    return textSurface
  
def message_clean(ptype):
    if ptype == 1:
    #清除第一个文字
        pygame.draw.rect(Main_screen, BG_Clolor, (0, display_height/2-55, display_width, 100), 0)
    #清除第二个文字
    else:
        pygame.draw.rect(Main_screen, BG_Clolor, (0, display_height-80, display_width, 100), 0)
    #刷屏
    pygame.display.update()
    time.sleep(0.001)
def diaplayKey(text):
    message_clean(1)
    #字体
    TextObj = pygame.font.Font('freesansbold.ttf',80)
    #第一个文字
    TextSurf= text_objects(text, TextObj)
    TextRect = TextSurf.get_rect()
    TextRect.center = ((display_width/2),(display_height/2))
    Main_screen.blit(TextSurf, TextRect)
    pygame.display.update()
    time.sleep(0.01)
def diaplayTouchMsg(text):
    message_clean(2)
    #字体
    TextObj = pygame.font.Font('freesansbold.ttf',20)
    #文字
    TextSurf= text_objects(text, TextObj)
    TextRect = TextSurf.get_rect()
    TextRect.center = ((display_width/2),(display_height-50))
    Main_screen.blit(TextSurf, TextRect)
    pygame.display.update()
    time.sleep(0.01)

# #定义点击器硬件点击头编号

HardDif = {}
# HardDif = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}

# #定义键盘和点击器点击头
keyDownDic = {K_0:'0',K_1:'1',K_0:'0',K_2:'2',K_0:'0',K_3:'3',K_0:'0',K_4:'4',K_0:'0',K_5:'5',K_0:'0',K_6:'6',K_0:'0',K_7:'7',K_0:'0',K_8:'8',K_0:'0',K_9:'9',K_a:'a',K_b:'b',K_c:'c',K_d:'d',K_e:'e',K_f:'f',K_g:'g',K_h:'h',K_i:'i',K_j:'j',K_k:'k',K_l:'l',K_m:'m',K_n:'n',K_o:'o',K_p:'p',K_q:'q',K_r:'r',K_s:'s',K_t:'t',K_u:'u',K_v:'v',K_w:'w',K_x:'x',K_y:'y',K_z:'z'}
# # keyDownDic = {K_w:1,K_s:2,K_a:3,K_d:4,K_y:5,K_u:6,K_i:7,K_o:8,K_h:9,K_j:10,K_k:11,K_l:12,K_c:13,K_v:14,K_b:15,K_n:16}

touchState = 0xffff

def hexTocmd(hexdat):
    ttmp = '%x'%(hexdat)
    tdstr = '<' + (4-len(ttmp))*'0' + ttmp + '>'
    return tdstr
def printDicData(dicdata):
    for k,v in dicdata.items():
        pstr = 'k:%s,'%(k)
        for i,d in enumerate(v):
            pstr += '%x,'%(d)
        print(pstr)

def initKeyDic():
    f = open('game.ini','r')
    lines = f.readlines()
    f.close()
    for i,v in enumerate(lines):
        v = v.replace('\n','').replace('\r','').replace('\t','').replace(' ','')
        tmps = v.split(':')
        keynum = tmps[0]
        tnum = tmps[1].split(',')
        tdtmp = 0xffff
        tutmp = 0
        for ti,tv in enumerate(tnum):
            ntv = int(tv)
            tutmp = (tutmp | (1<<(ntv-1)))
            tdtmp = (0xffff & (~ tutmp))
        # ttmp = '%x'%(tdtmp)
        # print('td:%s'%(ttmp))
        # tdstr = '<' + (4-len(ttmp))*'0' + ttmp + '>'
        # ttmp = '%x'%(tutmp)
        # print('tu:%s'%(ttmp))
        # tustr = '<' + (4-len(ttmp))*'0' + ttmp + '>'
        HardDif[keynum] = [tdtmp,tutmp]
    printDicData(HardDif)



def keyDown(kNumber,isHeaveTouch = True):
    global t
    global touchState
    if keyDownDic[kNumber] in HardDif:
        data = HardDif[keyDownDic[kNumber]][0]
        cmddata = ''
        if isHeaveTouch:
            touchState = touchState & data
            cmddata = hexTocmd(touchState)
            sendcmd(cmddata)       #发送点击头按下指令
        else:
            print('按键按下:%s'%(pygame.key.name(kNumber)))
        diaplayTouchMsg("J%s touch down,send->'%s'"%(keyDownDic[kNumber],cmddata))
    else:
        print('donw key:%s is not in ini file config'%(keyDownDic[kNumber]))

def keyUp(kNumber,isHeaveTouch = True):
    global t
    global touchState
    if keyDownDic[kNumber] in HardDif:
        data = HardDif[keyDownDic[kNumber]][1]
        cmddata = ''
        if isHeaveTouch:
            touchState = touchState | data
            cmddata = hexTocmd(touchState)
            sendcmd(cmddata)       #发送点击头弹起指令      
        else:
            print('按键弹起:%s'%(pygame.key.name(kNumber)))
        diaplayTouchMsg("J%s touch up,send->'%s'"%(keyDownDic[kNumber],cmddata))
    else:
        print('up key:%s is not in ini file config'%(keyDownDic[kNumber]))
def readconfig():
    f= open('com.ini')
    jstr = f.read()
    f.close()
    comstr = 'com'+jstr
    if sysSystem == 'Darwin':
        comstr = jstr
    return comstr

def main(isHeaveTouch = True):
    global t
    global Main_screen
    initKeyDic()
    btv = 115200                        #得到波特率
    dev = readconfig()
    t = serial.Serial(dev,115200,timeout=1)
    if t:
        pygame.init()
        Main_screen = pygame.display.set_mode((display_width, display_height), 0, 32)
        # bg = pygame.image.load(BG_IMAGE).convert()
        Main_screen.fill((0, 0, 0))
        # Main_screen.blit(bg, (0, 0))
        pygame.display.update()


        while 1:
            for event in pygame.event.get():
                # print(event.type)
                if event.type == QUIT:
                    exit()
                elif event.type == KEYDOWN:         #按键按下
                    print('KEYDOWN')
                    #event.key返回的是一个数字值，而K_LEFT,K_UP,K_RIGHT,K_DOWN等都是常量，
                    #他们代表的也是一个数字值，这些数字值可以用：print(event.key)获取到
                    #如：K_LEFT = 276
                    #  K_UP = 273
                    #所以下面的代码可以替换为：
                    #if event.key == 276:
                    # 
                    diaplayKey('%s Down'%(pygame.key.name(event.key)))
                    if event.key in keyDownDic:

                        keyDown(event.key,isHeaveTouch)

                    elif event.key == K_ESCAPE: #键盘ESC被按下,程序退出
                        exit()
                    else:
                        print('按键按下,但当前按键%s未绑定到点击器'%(pygame.key.name(event.key)))
                        message_clean(2)
                    
                elif event.type == KEYUP:           #按键弹起
                    print('KEYUP')
                    diaplayKey('%s Up'%(pygame.key.name(event.key)))
                    if event.key in keyDownDic:
                        keyUp(event.key,isHeaveTouch)
                    else:
                        print('按键弹起,但当前按键%s未绑定到点击器'%(pygame.key.name(event.key)))
                        message_clean(2)
                elif event.type == MOUSEMOTION:     #鼠标移动
                    # print('MOUSEMOTION')
                    pass
    else:
        print('init serial erro...')

if __name__ == '__main__':
    main()



