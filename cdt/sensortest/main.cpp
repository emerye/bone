#include <stdio.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#include "sensors.h"

#define LM75ADDRESS 0x4C

double lastTemp[3];

float getTemperature(int fd) {
	double degC;
	int read[3];
	long int average, raw;

	for (int i = 0; i < 1000; i++) {
		raw = wiringPiI2CReadReg16(fd, 0x00);
		read[i] = ((raw << 8) & 0xFF00) + (raw >> 8);
		sleep(0.1);
	}
	average = (read[0] + read[1] + read[2] + read[3] + read[4]) / 5;

	degC = (float) ((average / 32.0) / 8.0);
	return (float) (degC * 9.0 / 5.0 + 32);
}

double calcSteinHart(double resth) {

	double a = 1.327532184E-3;
	double b = 2.312481108E-4;
	double c = 1.177982663E-7;
	double tKelvin, oneOverK;

	// NTCLP100E3472H Metal pipe thermistor

	oneOverK = a + b * (log(resth)) + c * pow((log(resth)), 3);

	tKelvin = 1.0 / oneOverK;

	printf("Rth: %f deg K: %f deg C: %.1f  deg F: %.1f\n", resth, tKelvin,
			tKelvin - 273.15, ((tKelvin - 273.15) * 1.8) + 32);

	return (1.0 / oneOverK);

}

int main() {
	int ADCOFFSET = 0;
	int i2cHandle, fd = 0, i;
	float temperature;
	FILE *fp;
	char linebuf[256];
	float adcVoltage, resTh;
	// Character end pointer
	char* pend;

	while (1) {

		fp = fopen("/sys/class/hwmon/hwmon1/device/in4_input", "r");
		if (fp == NULL) {
			puts("ADC open failed.");
		}

		fgets(linebuf, 256, fp);
		fclose(fp);

		adcVoltage = strtof(linebuf, &pend);
		adcVoltage += ADCOFFSET;
		printf("ADC voltage %f\n", adcVoltage);
		//Rt = R0 * ((adcMax / adcVal) - 1)
		resTh = 4750.0 * ((3300.00 / adcVoltage) - 1);
		printf("Thermistor Resistance %f\n", resTh);
		calcSteinHart(resTh);
		fflush(stdout);
		sleep(1);

		puts("");
	}

	/*

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
	 */

	//calcSteinHart(4700);
	//calcSteinHart(1694);
	//calcSteinHart(15300);
	puts("Done\n");
	sleep(1);
	return 0;
}

