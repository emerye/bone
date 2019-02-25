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
#NXp datasheet
table11bit = ( 0x3F8, 0x3F7, 0x3F1, 0x3E8, 0xC8, 0x001, 0x00, 0x7FF, 0x738, 0x649, 0x648 )

def ctofBytes(msb, lsb):
  bitWidth = 11
  i = msb << 3 | lsb >> 5
  if (i < 1024):
      degC = i * 0.125
  else:
      degC = (i - int((i << 1) & 2**bitWidth)) * 0.125
  return (degC * 1.8 + 32)


tgtAddress = 0x4C
devobj = SMBus(1)
devobj.write_byte(tgtAddress, 0)

if debug:
  for i in table11bit:
      msb = i >> 3 & 0xFF
      lsb = ((i & 0xFF) << 5) & 0xFF
      if debug: print('11 Bit MSB %2x LSB %2x' % (msb, lsb))
      val = ctofBytes(msb, lsb)
     
for i in range(10):
    devobj.write_byte(tgtAddress, 0)
    with SMBusWrapper(1) as bus:
        block = bus.read_i2c_block_data(tgtAddress, 0, 2)
        data = bytes(block)
        msb = data[0]
        lsb = data[1] & 0xE0
        if debug: print('Bytes read from hardware: MSB %2x LSB %2x' % (msb, data[1]))
        tempF = ctofBytes(msb,lsb)
        print('Temperature is %.1f\n' % (tempF))
        time.sleep(1)
