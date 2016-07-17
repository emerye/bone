#!/bin/bash
runme=1
echo 49 > /sys/class/gpio/unexport
echo 49 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio49/direction
while [ $runme -eq 1 ]; do
echo 0 > /sys/class/gpio/gpio49/value
echo 1 > /sys/class/gpio/gpio49/value
done
