#!/usr/bin/env python3

import Adafruit_BBIO.ADC as adc 
import time
import math

def kelvin_to_c(kelvin):
    return (kelvin - 273.15);

def celsius_to_f(celsius):
    return (celsius * 1.8 + 32);


def calcTemperature_fromRes(r):

    a = 0.001307395;
    b = 0.000234655;
    c = 0.000000103;

    bterm = 0.0
    logr = 0.0;
    cterm = 0
    temp = 0
    tkelvin = 0;
    degc = 0;

    logr = math.log(r);
    bterm = b * logr;
    temp = logr * logr * logr;
    cterm = c * temp;
    temp = a + bterm + cterm
    tkelvin = 1 / temp
    temp = kelvin_to_c(tkelvin);
    return celsius_to_f(temp);
 
#main
adc.setup()
ref = 1.808
while (1): 
    adcvalue = adc.read("P9_36") * ref
    netcurrent = adcvalue / 5600
    calcTherRes = (ref - adcvalue) / netcurrent

    therRes = 5600 * ((ref / adcvalue) - 1)

    #print("CalcRes %d  OldCalcRes %d" % (calcTherRes, therRes))

    time.sleep(0.5)
    temperature = calcTemperature_fromRes(therRes)
    temperature += 2.5    #Offset
    print("Temperature: %d"  % (temperature))
adc.cleanup()


def kelvin_to_c(kelvin):
    return (kelvin - 273.15);
