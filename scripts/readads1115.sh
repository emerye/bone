#!/bin/sh

$volts

while true; do
    for i in 4; do
        echo -n "ch[$i]: "
        volts=`cat /sys/class/hwmon/hwmon1/device/in${i}_input`
    done

	y=$((volts))
	printf "%04d\n" $y	
	sleep .1
done
