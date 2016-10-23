#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <string.h>
#include <wiringPiSPI.h>


void gpioTest()
{
    int ret;
    int i, spifd;
    unsigned char outData[] = { 0x01, 0x55, 0xAA };
    unsigned char buffer[10];

    ret = wiringPiSetup();
    if (ret == -1) {
	printf("Error number %d\n", ret);
	perror("Opening wiring Pi");
    }
    pinMode(29, OUTPUT);
    pinMode(28, OUTPUT);

    spifd = wiringPiSPISetup(0, 1000000 * 2);
    if (spifd < 1) {
	perror("Error");
	return;
    }


    for (i = 0; i < 200000; i++) {

	memcpy(buffer, outData, 3);
	ret = wiringPiSPIDataRW(0, buffer, 3);
	if (ret < 1) {
	    perror("Error sending SPI data");
	}
	digitalWrite(29, LOW);
	digitalWrite(28, LOW);
	digitalWrite(29, HIGH);
	digitalWrite(28, HIGH);
//    usleep(1); 

    }
}



int main(int numArgs, char *args[])
{

    gpioTest();
    return 0;
}
