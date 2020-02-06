# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import sys
import time

temperaturepath='/sys/class/hwmon/hwmon2/temp1_input'


print('System {0}:'.format(sys.platform))

while True:
    temp_file = open(temperaturepath,'r')
    temp_read = temp_file.read()
    temp_read = temp_read.strip()
    print('Raw Read Data {0}'.format(temp_read))
    temp_file.close()
    tempf=float((int(temp_read))/1000)
    tempf=1.8 * tempf + 32.0
    print('Temperature Read: {:.1f} Deg F\n'.format(tempf))
    time.sleep(2.456)

    


