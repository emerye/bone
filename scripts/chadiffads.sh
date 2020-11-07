#!/bin/sh

$volts

while true; do
        echo -n "ch[$i]: "
        volts=`cat /sys/class/hwmon/hwmon1/device/in0_input`

	y=$((volts))
	printf "%04d\n" $y	
	sleep .1
done
