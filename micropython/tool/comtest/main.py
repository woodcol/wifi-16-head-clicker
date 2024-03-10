#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os,sys
from machine import UART
import time


uart = UART(0, baudrate=115200)

def fenliUART():
    uart.write('repl fenli\n')
    os.dupterm(None, 1)

def fujiaRELP():
    os.dupterm(uart, 1)
    uart.write('repl fujia\n')
# sss
def main():
    fenliUART()
    uart.write('hello')
    c = 120
    while c:
        c -= 1
        # uart.read(5) # read up to 5 bytes
        # d = sys.stdin.read()
        # print('stdin:%s\n'%(uartdat))
        d = uart.read(5)
        if d:
            dstr = d.decode()
            dstr = dstr.replace('\r','').replace('\n','')
            uart.write('d=%s\n'%(dstr))
            if dstr == 'q':
                print('is q')
                fujiaRELP()
                break
        time.sleep(1)
    fujiaRELP()

if __name__ == '__main__':
    main()