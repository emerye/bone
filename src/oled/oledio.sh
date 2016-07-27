#!/bin/bash

# echo spi > /sys/devices/platform/ocp/ocp:P9_18_pinmux/state
config-pin P9_17 spi
config-pin P9_18 spi
config-pin P9_21 spi
config-pin P9_22 spi
config-pin P9_14 gpio
config-pin P9_16 gpio
echo "Pin Configuration"
config-pin -q P9_17
config-pin -q P9_18
config-pin -q P9_21
config-pin -q P9_22
config-pin -q P9_14
config-pin -q P9_16
