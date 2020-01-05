
#include <stdio.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#include "sensors.h"


#define LM75ADDRESS 0x4C

double lastTemp[3];

float getTemperature(int fd)
 {
	double degC;
	int read[3];
	long int average, raw;

	for (int i=0; i<1000; i++)
	{
 	raw = wiringPiI2CReadReg16(fd, 0x00);
 	read[i]= ((raw << 8) & 0xFF00) + (raw >> 8);
 	sleep(0.1);
	}
	average = (read[0] + read[1] + read[2]+ read[3]+read[4])/5;


 	degC = (float)((average/ 32.0) / 8.0);
 	return (float)(degC * 9.0/5.0 + 32);
 }


int main() {
	int i2cHandle, fd = 0, i;
	float temperature;

	i2cHandle = wiringPiI2CSetup(LM75ADDRESS);
	if (i2cHandle < 0) {
		printf("Error opening I2C device device.\n");
	}
	Sensors lm75(fd, LM75ADDRESS);

	for (i = 0; i < 20; i++) {
		temperature = getTemperature(i2cHandle);
		printf("Temperature = : %.1f\n", temperature);
		fflush (stdout);
		sleep(1);
	}
	puts("Done\n");
	return 0;
}



