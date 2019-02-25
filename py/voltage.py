#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: root
"""

import time
from smbus2 import SMBus
from smbus2 import SMBusWrapper 
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
#    volts = commObj.read_word_data(tgtAddress, voltcmd)
#    print('Voltage: ', volts)
#    time.sleep(1.0)

    commObj.write_i2c_block_data(tgtAddress, 0x44, [2,2,0])
    verString = commObj.read_i2c_block_data(tgtAddress, 0x44, 13)
#    print( ' '.join( [ "%02x" % ord (x) for x in verString ] ))

#for x in verString:
#    print('0x%x ' % x, end="")


print('\n')


commObj.close()
