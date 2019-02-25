#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: root
"""

import time
from smbus2 import SMBus 

tgtAddress = 0x0B
voltcmd = 0x09

def printHex(data):
    hexChar = ''
    for val in data:
        hexChar += format('0x%02x ' % val) 

commObj = SMBus(2)
if commObj == None:
    print('Not able to open driver')
    quit()


for i in range(200): 
    b = commObj.read_word_data(tgtAddress, voltcmd)
    #print('Byte Word: ', b)

    commObj.write_i2c_block_data(tgtAddress, 0x44, [2,0])
    verString = commObj.read_i2c_block_data(tgtAddress, 0x44, 13)
    print('Result ', verString)

commObj.close()
