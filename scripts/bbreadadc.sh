#!/bin/bash

while [ 1 ] 
do
cat /sys/bus/iio/devices/iio:device0/in_voltage1_raw
sleep 1
done
