#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: root
"""

import time
from smbus2 import SMBus 
from ctypes import *

tgtAddress = 0x0B
SBSCurrent = 0x0A
SBSAveCurrent = 0x0B

voltcmd = 0x09

def printHex(data):
    hexChar = ''
    for val in data:
        hexChar += format('0x%02x ' % val) 

commObj = SMBus(2)
if commObj == None:
    print('Not able to open driver')
    quit()

curList = list()
avecurList = list()
cur = c_short()
for i in range(100): 
    cur = commObj.read_word_data(tgtAddress, SBSCurrent)
    avecurr = commObj.read_word_data(tgtAddress, SBSAveCurrent)
    current = c_short(cur).value
    aveCurrent = c_short(avecurr).value
    curList.append(current)
    avecurList.append(aveCurrent)
    print('Current: ', current, 'Average Current', aveCurrent)
    time.sleep(1.0)

mincur = min(curList)
maxcur = max(curList)
print('Lowest current: %d Highest Current %d' % (mincur, maxcur))

minaveCurr = min(avecurList)
maxaveCurr = max(avecurList)
print('Lowest Ave current: %d Highest Ave Current %d' % (minaveCurr, maxaveCurr))
'''
    commObj.write_i2c_block_data(tgtAddress, 0x44, [2,0])
    verString = commObj.read_i2c_block_data(tgtAddress, 0x44, 13)
    print('Result ', verString)
    '''

commObj.close()
