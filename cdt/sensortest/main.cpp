
#include <stdio.h>
#include <wiringPiI2C.h>

#include "sensors.h"


#define LM75ADDRESS 0x48

float getTemperature(int fd)
 {
	float degC, degF;

 	int raw = wiringPiI2CReadReg16(fd, 0x00);
 	raw = ((raw << 8) & 0xFF00) + (raw >> 8);
 	degC = (float)((raw / 32.0) / 8.0);
 	return (float)(degC * 9.0/5.0 + 32);
 }


int main() {
	int i2cHandle, fd;
	float temperature;

	i2cHandle = wiringPiI2CSetup(LM75ADDRESS);
	if (i2cHandle < 0) {
		printf("Error opening device.\n");
	}
	Sensors lm75(fd, LM75ADDRESS);


	temperature = getTemperature(i2cHandle);
	printf("Temperature = : %g", temperature);
	return 0;
}



