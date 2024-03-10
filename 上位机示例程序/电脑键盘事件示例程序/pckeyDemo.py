#!/usr/bin/python
# -*- coding: utf-8 -*-
#创建SocketServerTCP服务器：
import os
import socket
import time
import platform
from sys import exit

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


host = '192.168.100.200' #板子连上路由器之后分配的IP地址
port = 23                #板子的端口号,这个是固定烧写在板子里的,不用动

#'!'工作模式
#['1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g']
#'@'工作模式
#['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z']

#这个函数是'!'工作模式下用来把延时毫秒数转换为要发送要字符串命令
def intToHexStrTime(dtime):
    strhex = str(hex(dtime))[2:]
    if len(strhex) == 3:
        strhex = '[0' + strhex + ']'
    elif len(strhex) == 2:
        strhex = '[00' + strhex + ']'
    return strhex


'''
  事件             产生途径              参数
  QUIT         用户按下关闭按钮          none
  ATIVEEVENT         Pygame被激活或者隐藏          gain, state
  KEYDOWN         键盘被按下              unicode, key, mod
  KEYUP         键盘被放开              key, mod
  MOUSEMOTION         鼠标移动              pos, rel, buttons
  MOUSEBUTTONDOWN     鼠标按下              pos, button
  MOUSEBUTTONUP     鼠标放开              pos, button
  JOYAXISMOTION     游戏手柄(Joystick or pad)移动      joy, axis, value
  JOYBALLMOTION     游戏球(Joy ball)?移动      joy, axis, value
  JOYHATMOTION     游戏手柄(Joystick)?移动      joy, axis, value
  JOYBUTTONDOWN     游戏手柄按下              joy, button
  JOYBUTTONUP         游戏手柄放开              joy, button
  VIDEORESIZE         Pygame窗口缩放          size, w, h
  VIDEOEXPOSE         Pygame窗口部分公开(expose)      none
  USEREVENT         触发了一个用户事件          code
'''


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

#定义点击器硬件点击头编号
HardDif = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}

#定义键盘和点击器点击头
keyDownDic = {K_w:1,K_s:2,K_a:3,K_d:4,K_y:5,K_u:6,K_i:7,K_o:8,K_h:9,K_j:10,K_k:11,K_l:12,K_c:13,K_v:14,K_b:15,K_n:16}


#               电脑键盘排列
#
#       w                       y   u   i   o
#   a       d                   h   j   k   l 
#       s                   
#               c   v   b   n

#               点击器对应按键
#
#       1                       5   6   7   8           
#   3       4                   9   10  11  12
#       2
#               13  14  15  16
#

def keyDown(client_socket,kNumber,isHeaveTouch = True):
    sendCmd = HardDif[keyDownDic[kNumber]][0]
    if isHeaveTouch:
        client_socket.send(sendCmd.encode())       #发送点击头按下指令
    else:
        print('按键按下:%s'%(pygame.key.name(kNumber)))
    diaplayTouchMsg("J%d touch down,send->'%s'"%(keyDownDic[kNumber],sendCmd))

def keyUp(client_socket,kNumber,isHeaveTouch = True):
    sendCmd = HardDif[keyDownDic[kNumber]][1]
    if isHeaveTouch:
        client_socket.send(sendCmd.encode())       #发送点击头弹起指令
    else:
        print('按键弹起:%s'%(pygame.key.name(kNumber)))
    diaplayTouchMsg("J%d touch up,send->'%s'"%(keyDownDic[kNumber],sendCmd))



def main(isHeaveTouch = True):
    client_socket = None
    global Main_screen
    if isHeaveTouch:
        client_socket = socket.socket()      # 创建连接板子的客户端socket对象
        client_socket.connect((host, port))  # 连接到板子上

        client_socket.send('@'.encode())  #切换为'!'工作模式
        time.sleep(0.1) #延时100ms

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

                    keyDown(client_socket,event.key,isHeaveTouch)

                elif event.key == K_ESCAPE: #键盘ESC被按下,程序退出
                    exit()
                else:
                    print('按键按下,但当前按键%s未绑定到点击器'%(pygame.key.name(event.key)))
                    message_clean(2)
                
            elif event.type == KEYUP:           #按键弹起
                print('KEYUP')
                diaplayKey('%s Up'%(pygame.key.name(event.key)))
                if event.key in keyDownDic:
                    keyUp(client_socket,event.key,isHeaveTouch)
                else:
                    print('按键弹起,但当前按键%s未绑定到点击器'%(pygame.key.name(event.key)))
                    message_clean(2)
            elif event.type == MOUSEMOTION:     #鼠标移动
                # print('MOUSEMOTION')
                pass

def test():
    main()

if __name__ == '__main__':
    # main()
    test()



