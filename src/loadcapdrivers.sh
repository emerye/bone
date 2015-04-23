#!/bin/bash
# SPI1
echo "BB-SPI1-STD-01" > /sys/devices/bone_capemgr.9/slots
# GPIO 12
# echo "BB-GPIOP912" > /sys/devices/bone_capemgr.9/slots
# GPIO 48 50 51
echo "BB-GPIOP14P15P16" > /sys/devices/bone_capemgr.9/slots
