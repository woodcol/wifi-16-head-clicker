# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import uos, machine
#uos.dupterm(None, 1) # disable REPL on UART(0)
import gc
import network
import webrepl
webrepl.start()
gc.collect()

sta_if = network.WLAN(network.STA_IF)  # 配置wifi模式为station
sta_if.active(False)