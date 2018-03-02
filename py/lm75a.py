#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Feb  9 19:33:10 2018

@author: root
"""

import time
import ctypes as cvarTypes
from smbus import SMBus
from smbus2 import SMBusWrapper

debug = False

tableValue = ( 0xFA, 0x32, 0x1, 0, 0x01FF, 0x01CE, 0x0192)

def ctofBytes(msb, lsb):
    '''Convert LM75A data '''
    degC = msb
    if (msb >> 7) == 1:
        negValue = (0xFF << 8) + msb
        val = cvarTypes.c_int16(negValue)
        degC = val.value
        
    if (lsb & 0x80) == 0x80:
        degC = degC + 0.5
        
    retValue = degC * 1.8 + 32
    if debug:
        print('DegC %.1f DegF %.1f' % (degC, retValue)) 
        
    return retValue

tgtAddress = 0x4C
devobj = SMBus(1)
devobj.write_byte(tgtAddress, 0)


if debug:
  for i in tableValue: 
    print('I',i)
    if (i % 2) == 1:
       lsb = 0x80
    else:
       lsb = 0
            
    msb = i >> 1
    if debug: print('MSB %2x LSB %2x' % (msb, lsb))
    val = ctofBytes(msb, lsb)
    print('ctoBytes Temperature is %.1f\n' % (val))

   
for i in range(9):
    devobj.write_byte(tgtAddress, 0)
    with SMBusWrapper(1) as bus:
        block = bus.read_i2c_block_data(tgtAddress, 0, 2)
        data = bytes(block)
        msb = data[0]
        lsb = data[1] & 0x80
        if debug: print('MSB %2x LSB %2x' % (msb, lsb))
        tempF = ctofBytes(msb,lsb)
        print('Temperature is %.1f\n' % (tempF))
        time.sleep(1)
