#!/usr/bin/python3

import sys 
import time
import numpy as np

volts = [0,1,2,3,4,5,6,7,8,9]
position = 0

def getvolts():
	global position
	global volts

	f = open("/sys/class/hwmon/hwmon1/device/in4_input", "r")
	reading = f.read() 
	reading = reading.strip()
	f.close()
	volts[position] = reading 
	sum = 0
	for i in range(0,10):
		sum += int(volts[i])

	average = sum / 10
	print("Average ", "%d" % (average))
        	
	position += 1
	if position == 10:
		position = 0; 

while 1:
	print(volts)
	getvolts()
	time.sleep(1)

