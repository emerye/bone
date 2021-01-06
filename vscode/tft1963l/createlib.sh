#!/bin/bash 

g++ -shared -o libtft1963.so tft1963.o glcdfont.o DispDraw.o Adafruit_GFX.o

# Static library
ar -rc libtft1963.a tft1963.o glcdfont.o DispDraw.o Adafruit_GFX.o 

sudo cp libtft1963.so /usr/local/lib

sudo ldconfig -v
