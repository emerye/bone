from machine import ADC, Pin
import machine
import lcd
import time
import math
import tm1638 

def adcReading(slope, offset, adcCount):
    return ((offset + adcCount) / slope)

def adc0dbbitRead(samples):
    accum = 0
    for i in range(0,samples):
        adcRead = adc.read()
        accum = accum + adcRead
        time.sleep(0.005)
    adcCnt = accum/samples
    return(adcReading(4287.71384, 275.16018, adcCnt))

def adc6dbbitRead(samples):
    accum = 0
    for i in range(0,samples):
        adcRead = adc.read()
        accum = accum + adcRead
        time.sleep(0.005)
    adcCnt = accum/samples
    return(adcReading(2338.47340, 209.62143, adcCnt))

def kelvin_to_c(kelvin):
    return (kelvin - 273.15)

def kelvin_to_c(kelvin):
    return (kelvin - 273.15)

def celsius_to_f(celsius):
    return (celsius * 1.8 + 32)

def calcTemperature_fromRes(r):
    a = 0.001307395
    b = 0.000234655
    c = 0.000000103

    logr = math.log(r)
    bterm = b * logr
    temp = logr * logr * logr
    cterm = c * temp
    temp = a + bterm + cterm
    tkelvin = 1 / temp
    temp = kelvin_to_c(tkelvin)
    return celsius_to_f(temp)
 
def getTemperature(): 
    ref = 2.5 
    adcvalue = adc6dbbitRead(100)  
    netcurrent = adcvalue / 3930 
    calcTherRes = (ref - adcvalue) / netcurrent
    therRes = 3930 * ((ref / adcvalue) - 1)
    temperature = calcTemperature_fromRes(therRes)
    temperature += 1.0    #Offset
    return temperature

def adcCharacterize():
    allCounts = 0
    data = []
    for i in range(0, 100):
        adcValue = adc.read()         
        data.append(adcValue)
        allCounts = allCounts + adcValue 
        time.sleep(0.005)

    buf = "Ave:%d L:%d" % (allCounts/100,adcValue)
    lcdobj.WriteString(0, 20, buf)
    buf = "Max:%d Min:%d" % (max(data), min(data))
    lcdobj.WriteString(1, 20, buf)

# main
lcdobj = lcd.CharLCD(0x27)
inputPin = Pin(4, Pin.IN, Pin.PULL_UP)

tm = tm1638.TM1638(stb=Pin(16), clk=Pin(22), dio=Pin(23))

adc = ADC(Pin(32)) 
adc.atten(ADC.ATTN_6DB) 
adc.width(ADC.WIDTH_12BIT)   # set 12 bit

while inputPin.value() == 1: 
    tempf = getTemperature()
    reading = '%.1f deg' % tempf 
    lcdobj.WriteString(0,0,reading)
    tm.temperature(int(tempf + 0.5),0)

sys.exit()
