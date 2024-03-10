# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import os,sys


m_tST = 0xffff
#字符串转为hex十六进制数
# hex_string = "0xFF"

# an_integer = int(hex_string, 16)

# hex_value = hex(an_integer)
# print(hex_value)

#使用两个字节码控制多个点击头按下
def touchCMD(cmd):
    # hex_string = '0x' + cmd
    an_integer = int(cmd, 16)
    print(type(an_integer),an_integer)
    hex_value = hex(an_integer)
    print(type(hex_value),hex_value)

#使用字节码控制多个点击头抬起
def unTouchCMD(cmd):
    an_integer = int(cmd, 16)
    print(type(an_integer),an_integer)
    hex_value = hex(an_integer)
    print(type(hex_value),hex_value)

def main():
    touchCMD('FFFE')

if __name__ == '__main__':  
    main()
    