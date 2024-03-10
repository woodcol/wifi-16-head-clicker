#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2022-11-01 14:06:58
# @Author  : Your Name (you@example.org)
# @Link    : http://example.org
# @Version : $Id$

import time
import os
import network
import espnow

# A WLAN interface must be active to send()/recv()

def main():
    sta = network.WLAN(network.STA_IF)  # Or network.AP_IF
    sta.active(True)
    sta.disconnect()   # For ESP8266

    e = espnow.ESPNow()
    e.active(True)
    peer = b'\xd8\xbf\xc0\xff\x53\xbc'   # MAC address of peer's wifi interface
    # bcast = b'\xff'*6   # MAC address of peer's wifi interface
    # e.add_peer(peer)
    e.add_peer(peer)

    # e.send("Starting...")       # Send to all peers
    i = 0
    while True:
        print('start send...')
        e.send(peer, str(i), True)
        e.send(peer,b'end')
        i += 1
        if i > 9:
            i = 0
        time.sleep(3)

if __name__ == '__main__':
    main()