#!/bin/bash 

g++ -shared -o libtft1963.so tft1963.o glcdfont.o DispDraw.o Adafruit_GFX.o

