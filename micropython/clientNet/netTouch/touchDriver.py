# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import machine
import time
#uos.dupterm(None, 1) # disable REPL on UART(0)

#_OE_  16  //GPIO16,输出使能三态门控制端，低电平为开，高电平为关
#_RCK_ 14  //GPIO14,输入数据锁存到输出的控制脉冲，控制为下降沿触发数据锁存(芯片为上升沿，但有三极管反相)
#_SRCK_ 13 //GPIO13,输入数据移位动作控制脉冲，控制为下降沿动作(芯片为上升沿，但有三极管反相)
#_SER_ 12  //GPIO12,移位输入数据，在SRCK有控制脉冲时，这里的数据会被移入寄存器(实际移入数据这此数据相反，但有三极管反相)

#点击器硬件驱动程序
#点击器动作控制硬件类,这个类只能有一个实例对象来控制点击器
class TouchObj:
    def __init__(self):
        self.key = machine.Pin(2, machine.Pin.IN)
        self.oe = machine.Pin(16, machine.Pin.OUT)
        self.rck = machine.Pin(14, machine.Pin.OUT)
        self.srck = machine.Pin(13, machine.Pin.OUT)
        self.ser = machine.Pin(12, machine.Pin.OUT)
        self.isTestTrue = True
        self.m_data = 0x0ffff
        self.isDelayM = False
        self.mDelay = 1
        self.uDelay = 10
        self.initTouch()
#输出引脚硬件电平初始化
    def initObj(self):
        time.sleep_us(self.uDelay)
        self.ser.off()
        time.sleep_us(self.uDelay)
        self.rck.on()
        time.sleep_us(self.uDelay)
        self.srck.on()
        time.sleep_us(self.uDelay)
        self.oe.on()
#串行发送一个值到串行输出硬件状态锁存
    def sendStep(self,bdat):
        self.ser.value(not bdat)
        time.sleep_us(self.uDelay)
        self.srck.on()
        time.sleep_us(self.uDelay)
        self.srck.off()
        time.sleep_us(self.uDelay)
        self.srck.on()
        time.sleep_us(self.uDelay)
        self.ser.off()

#将硬件锁存中的按键状态执行到实际按键上,这里不用管为纯外置硬件上的操作
    def updateOUT(self):
        self.rck.on()
        time.sleep_us(self.uDelay)
        self.rck.off()
        time.sleep_us(self.uDelay)
        self.rck.on()
        time.sleep_us(self.uDelay)
#初始化所有按键为非按下
    def initTouch(self):
        self.initObj()
        self.clearAllTouch()
#设置self.m_data中的pNumber按键号的状态
#这个的state为False或者0表示键非按下,为非0或者True表示为按下
    def setPin(self,pNumber,state):
        if state:
            self.m_data = self.m_data | (0x001<<pNumber)
        else:
            self.m_data = self.m_data & (~(0x001<<pNumber))

#将self.m_data数值中的按键状态更新到硬件实际状态中
    def updateData(self):
        tmpdat = self.m_data
        for i in range(16):
            st = (tmpdat<<i)&0x08000
            self.sendStep(st)
        self.updateOUT()
#使用一个数直接设置所有引脚状态的数值
    def set16Pins(self,states):
        self.m_data = states
#设置所有按键为非按状态的数值,
    def clearAllTouch(self):
        self.m_data = 0x0ffff
        self.updateData()
#设置所有按键为按下状态
    def setAllTouch(self):
        self.m_data = 0x0
        self.updateData()
#测试
    def test(self):
        if self.isTestTrue:
            self.ser.off()
            self.isTestTrue = False
        else:
            self.ser.on()
            self.isTestTrue = True
        if self.isDelayM:
            time.sleep_ms(self.mDelay)
        else:
            time.sleep_us(self.uDelay)
        self.srck.on()
        if self.isDelayM:
            time.sleep_ms(self.mDelay)
        else:
            time.sleep_us(self.uDelay)
        self.srck.off()
        if self.isDelayM:
            time.sleep_ms(self.mDelay)
        else:
            time.sleep_us(self.uDelay)
        self.srck.on()
        if self.isDelayM:
            time.sleep_ms(self.mDelay)
        else:
            time.sleep_us(self.uDelay)
        self.rck.on()
        if self.isDelayM:
            time.sleep_ms(self.mDelay)
        else:
            time.sleep_us(self.uDelay)
        self.rck.off()
        if self.isDelayM:
            time.sleep_ms(self.mDelay)
        else:
            time.sleep_us(self.uDelay)
        self.rck.on()
#获取当前按键状态
    def getStates(self):
        return self.m_data