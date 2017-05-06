
#include <stdio.h>
#include <wiringPiI2C.h>
#include "sensors.h"


Sensors::Sensors(int i2cHandle, int i2cAddress) {
	this->i2cHandle = i2cHandle;
	this->i2cAddress = i2cAddress;
}

Sensors::~Sensors() {

}


float Sensors::getLM75Temperature()
 {
	float degC;

 	int raw = wiringPiI2CReadReg16(i2cHandle, 0x00);
 	raw = ((raw << 8) & 0xFF00) + (raw >> 8);
 	degC = (float)((raw / 32.0) / 8.0);
 	return (float)(degC * 9.0/5.0 + 32);
 }

