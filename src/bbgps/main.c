#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <nmea/nmea.h>
#include <string.h>
#include "serial.h"
#include "lcdchar.h"

//install packages i2c-tools, libi2c-dev for i2c

int i2caddr = 0x27;
unsigned char i2creg = 0;
int i2cfd;
char timeBuff[50];
char dateBuff[50];

char *GetTime()
{
    time_t curTime;
    struct tm *timeInfo;

    time(&curTime);
    timeInfo = localtime(&curTime);
    strftime(timeBuff, 50, "%r", timeInfo);
    strftime(dateBuff, 50, "%D", timeInfo);
    return timeBuff;
}


void Display(int i2cfd, unsigned char tgtAddress)
{
    int r;

    r = ioctl(i2cfd, I2C_SLAVE, tgtAddress);
    if (r < 0) {
	perror("Selecting i2c device.\n");
    }
    GetTime();
    WriteString(i2cfd, 0, 0, dateBuff);
    WriteString(i2cfd, 1, 0, GetTime());
}


int initi2c()
{
    int r;
    int i2cfd;
    char *dev = "/dev/i2c-2";

    i2cfd = open(dev, O_RDWR);
    if (i2cfd < 0) {
	perror("Opening i2c device node.\n");
	return -1;
    }
    r = ioctl(i2cfd, I2C_SLAVE, i2caddr);
    if (r < 0)
	perror("Selecting i2c device.\n");

    Setup4bit(i2cfd);
    DisplayClear(i2cfd);
    WriteString(i2cfd, 0, 0, "Hellow"); 
    return 0;
}


int main()
{
    const char *buff[] = {
        "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69\r\n",
        "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,0.0,0000*70\r\n",
        "$GPGSV,2,1,08,01,05,005,80,02,05,050,80,03,05,095,80,04,05,140,80*7f\r\n",
        "$GPGSV,2,2,08,05,05,185,80,06,05,230,80,07,05,275,80,08,05,320,80*71\r\n",
        "$GPGSA,A,3,01,02,03,04,05,06,07,08,00,00,00,00,0.0,0.0,0.0*3a\r\n",
        "$GPRMC,111609.14,A,5001.27,N,3613.06,E,11.2,0.0,261206,0.0,E*50\r\n",
        "$GPVTG,217.5,T,208.8,M,000.00,N,000.01,K*4C\r\n"
    };

    int it;

    nmeaINFO info;
    nmeaPARSER parser;

    initi2c();  
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    for(it = 0; it < 6; ++it) {
        nmea_parse(&parser, buff[it], (int)strlen(buff[it]), &info);
        printf("Sentence %s\n", (char *) buff[it]); 
        printf("Lat %f\n", info.lat); 
        }

    nmea_parser_destroy(&parser);

   close(i2cfd); 
    return 0;
}
