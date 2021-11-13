#!/usr/bin/env python3

import Adafruit_BBIO.GPIO as GPIO

GPIO.setup("P8_46",GPIO.OUT)
GPIO.output("P8_46",GPIO.HIGH)
GPIO.cleanup()
