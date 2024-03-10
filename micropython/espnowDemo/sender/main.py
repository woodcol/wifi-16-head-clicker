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
import random

#实始化一个点击器控制实例对象
tobj = t.TouchObj()

RUNSTATE = False

#短按函数
def shotDown(etool,peers):
    if RUNSTATE:
        for i,v in enumerate(peers):
            etool.send(v, b'0')
            host, msg = etool.recv()
            if host == v:
                print(host,msg)
                continue
    else:
        for i,v in enumerate(peers):
            etool.send(v, b'1')
            host, msg = etool.recv()
            if host == v:
                print(host,msg)
                continue


# A WLAN interface must be active to send()/recv()
def main():
    sta = network.WLAN(network.STA_IF)  # Or network.AP_IF
    sta.active(True)
    sta.disconnect()   # For ESP8266

    e = espnow.ESPNow()
    e.active(True)
    peers = []
    peers.append(b'\xd8\xbf\xc0\xff\x53\xbc')   # 第1个子客户端
    peers.append(b'\xd8\xbf\xc0\xff\x53\xbc')   # 第2个子客户端
    peers.append(b'\xd8\xbf\xc0\xff\x53\xbc')   # 第3个子客户端
    peers.append(b'\xd8\xbf\xc0\xff\x53\xbc')   # 第4个子客户端
    for i,v in enumerate(peers):
        e.add_peer(v)
    print('start contorler...')
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
                keyuptime = 0
                shotDown(e,peers)
        else:
            time.sleep_ms(10)
        

if __name__ == '__main__':
    main()