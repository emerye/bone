#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sun Feb  4 20:46:20 2018

@author: andy
"""
import RPi.GPIO as GPIO
import time


# Pin Definitons:
ledPin = 11 #GPIO 0

import RPi.GPIO as GPIO

# Pin Setup:
GPIO.setmode(GPIO.BOARD)
mode = GPIO.getmode()
if (mode == GPIO.BOARD):
    print('GPIO in BOARD mode')
    
GPIO.setup(ledPin, GPIO.OUT)
time.sleep(1)
for cnt in range(3):
    GPIO.output(ledPin,GPIO.HIGH)
    time.sleep(2)
    GPIO.output(ledPin,GPIO.LOW)
    time.sleep(2)
#GPIO.cleanup()

print ('Complete')