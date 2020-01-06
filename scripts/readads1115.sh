#!/bin/sh


while true; do
    for i in 4; do
        echo -n "ch[$i]: "
        cat /sys/class/hwmon/hwmon1/device/in${i}_input
    done

    echo ""
    sleep 1
done
