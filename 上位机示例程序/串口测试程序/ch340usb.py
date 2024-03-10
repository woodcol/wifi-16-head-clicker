#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2019-01-21 15:37:52
# @Author  : Your Name (you@example.org)
# @Link    : http://example.org
# @Version : $Id$

import os,sys,subprocess

def getUSB():
    cmd = '/bin/ls /dev/cu.usbserial*'
    # out = os.system('/bin/ls /dev/cu*')
    out = subprocess.getoutput(cmd)
    outlist = out.split('\n')
    if len(outlist) == 1:
        if outlist[0].find(' ') !=-1 or outlist[0].find('cu') ==-1:
            print('erro')
            return
        print(outlist[0])
        return outlist[0]
    else:
        print('erro')

if __name__ == '__main__':
    getUSB()