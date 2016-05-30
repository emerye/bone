#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include <linux/serial.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <nmea/nmea.h>
#include "serial.h"
#include "lcdchar.h"
#include "itimer.h"

#define DEBUG 1

struct sigaction sa;
struct itimerval timer;

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B9600
	/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1		/* POSIX compliant source */

#define FALSE 0
#define TRUE 1

static char *compassDirection[] =
    { "  N", "NNE", " NE", "ENE", "  E", "ESE",
    " SE", "SSE", "  S", "SSW", " SW", "WSW", "  W", "WNW", " NW", "NNW",
    "  N", "ZZZ" 
};

char buf[255];
int i2caddr = 0x27;
unsigned char i2creg = 0;
int i2cfd;
char timeBuff[50];
char dateBuff[50];
unsigned int startupCnt = 0;

char *GetTime();

double speedaverage = 0;
unsigned int count;
unsigned int cumulativeCount;

int fd;				//file descriptor
nmeaINFO info;
nmeaPARSER parser;

// Calculate average speed
double AverageSpeed(double currentSpeed)
{
    speedaverage = speedaverage + (currentSpeed - speedaverage) / count;
    return speedaverage;
}

//Return index into array
char *compass2direction(double bearing)
{
    if (bearing > 348.75 || bearing < 11.25) {
	return compassDirection[0];
    } else if (bearing >= 11.25 && bearing < 33.75) {
	return compassDirection[1];
    } else if (bearing >= 33.75 && bearing < 56.25) {
	return compassDirection[2];
    } else if (bearing >= 56.25 && bearing < 78.75) {
	return compassDirection[3];
    } else if (bearing >= 78.75 && bearing < 101.25) {
	return compassDirection[4];
    } else if (bearing >= 101.25 && bearing < 123.75) {
	return compassDirection[5];
    } else if (bearing >= 123.75 && bearing < 146.25) {
	return compassDirection[6];
    } else if (bearing >= 146.25 && bearing < 168.75) {
	return compassDirection[7];
    } else if (bearing >= 168.75 && bearing < 191.25) {
	return compassDirection[8];
    } else if (bearing >= 191.25 && bearing < 213.75) {
	return compassDirection[9];
    } else if (bearing >= 213.75 && bearing < 236.25) {
	return compassDirection[10];
    } else if (bearing >= 236.25 && bearing < 258.75) {
	return compassDirection[11];
    } else if (bearing >= 258.75 && bearing < 281.25) {
	return compassDirection[12];
    } else if (bearing >= 281.25 && bearing < 303.75) {
	return compassDirection[13];
    } else if (bearing >= 303.75 && bearing < 326.25) {
	return compassDirection[14];
    } else if (bearing >= 326.25 && bearing < 348.75) {
	return compassDirection[15];
    }
    return compassDirection[16]; //Error
}

void process_nmea(char *sentence, int length)
{
    nmeaTIME nmeaTime;
    char senCode[10];
    char speed[30];
    char alt[30];
    char buffer[30];
    int currentSpeed, avgSpeed;
    double course;

    if (length < 10) {
	printf("Sentence length is incorrect\n");
	return;
    }

    startupCnt++;
    if (startupCnt < 10) {
	startupCnt++;
	return;
    }

    strncpy(senCode, sentence, 6);
    if ((strncmp(senCode, "$GPGGA", 6) == 0)) {
	nmea_parse(&parser, sentence, length, &info);
	nmea_time_now(&nmeaTime);
	printf("Hour %d min %d\n", nmeaTime.hour, nmeaTime.min);
	printf("GGA Lat %f Lon %f\n", info.lat, info.lon);
	printf("Altitude %f ft\n", info.elv * 3.28084);
	sprintf(alt, "%4d", (int) (info.elv * 3.28084));
	WriteString(i2cfd, 2, 18, "ft");
	WriteString(i2cfd, 2, 14, alt);
    } else if ((strncmp(senCode, "$GPGSA", 6) == 0)) {
	nmea_parse(&parser, sentence, length, &info);
    } else if ((strncmp(senCode, "$GPGSV", 6) == 0)) {
	nmea_parse(&parser, sentence, length, &info);
    } else if ((strncmp(senCode, "$GPRMC", 6) == 0)) {
	nmea_parse(&parser, sentence, length, &info);
	count++;
	currentSpeed = (int) (info.speed * 0.621371);

	sprintf(buffer, "Lat: %.4f ", info.lat);
	WriteString(i2cfd, 0, 0, buffer);

	sprintf(buffer, "S%2d", info.satinfo.inview);
	WriteString(i2cfd, 0, 17, buffer);

	sprintf(buffer, "Lon: %.4f", info.lon);
	WriteString(i2cfd, 1, 0, buffer);

	avgSpeed = (int) AverageSpeed(currentSpeed);
	sprintf(speed, "%2dmph %2dmph ", currentSpeed, avgSpeed);
	WriteString(i2cfd, 2, 0, speed);

	sprintf(buffer, "%3dmin", (int) ((count / 60.0) + 0.5));
	WriteString(i2cfd, 3, 0, buffer);

	course = info.direction - info.declination;
	sprintf(buffer, "%3d", (int) course);
	WriteString(i2cfd, 3, 12, buffer);

	sprintf(buffer, "%s", compass2direction(course));
	WriteString(i2cfd, 3, 17, buffer);

    } else if ((strncmp(senCode, "$GPVTG", 6) == 0)) {
    }
}


void initserialport()
{
    struct termios oldtio, newtio;
    /* 
       Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C.
     */
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
    if (fd < 0) {
	perror(MODEMDEVICE);
	exit(-1);
    }
    tcgetattr(fd, &oldtio);	/* save current serial port settings */
    bzero(&newtio, sizeof(newtio));	/* clear struct for new port settings */

    /* 
       BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
       all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters
     */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

    /*
       IGNPAR  : ignore bytes with parity errors
       ICRNL   : map CR to NL (otherwise a CR input on the other computer
       will not terminate input)
       otherwise make device raw (no other input processing)
     */
    newtio.c_iflag = IGNPAR | ICRNL;

    /*
       Raw output.
     */
    newtio.c_oflag = 0;

    /*
       ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program
     */
    newtio.c_lflag = ICANON;

    /* 
       initialize all control characters 
       default values can be found in /usr/include/termios.h, and are given
       in the comments, but we don't need them here
     */
    newtio.c_cc[VINTR] = 0;	/* Ctrl-c */
    newtio.c_cc[VQUIT] = 0;	/* Ctrl-\ */
    newtio.c_cc[VERASE] = 0;	/* del */
    newtio.c_cc[VKILL] = 0;	/* @ */
    newtio.c_cc[VEOF] = 4;	/* Ctrl-d */
    newtio.c_cc[VTIME] = 0;	/* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;	/* blocking read until 1 character arrives */
    newtio.c_cc[VSWTC] = 0;	/* '\0' */
    newtio.c_cc[VSTART] = 0;	/* Ctrl-q */
    newtio.c_cc[VSTOP] = 0;	/* Ctrl-s */
    newtio.c_cc[VSUSP] = 0;	/* Ctrl-z */
    newtio.c_cc[VEOL] = 0;	/* '\0' */
    newtio.c_cc[VREPRINT] = 0;	/* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0;	/* Ctrl-u */
    newtio.c_cc[VWERASE] = 0;	/* Ctrl-w */
    newtio.c_cc[VLNEXT] = 0;	/* Ctrl-v */
    newtio.c_cc[VEOL2] = 0;	/* '\0' */

    /* 
       now clean the modem line and activate the settings for the port
     */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
}


int readSerialString()
{
    int res;
    /*
       terminal settings done, now handle input
       In this example, inputting a 'z' at the beginning of a line will 
       exit the program.
     */
    /* read blocks program execution until a line terminating character is 
       input, even if more than 255 chars are input. If the number
       of characters read is smaller than the number of chars available,
       subsequent reads will return the remaining chars. res will be set
       to the actual number of characters actually read */
    do {
	res = read(fd, buf, 255);
    } while (res < 10);
    //buf[res] = 0;         /* set end of string, so we can printf */
    //if (res > 6) printf(":%s Count: %d\n", buf, res);

    /* restore the old port settings */
    //tcsetattr(fd, TCSANOW, &oldtio);
    return res;
}


void dump_data(unsigned char *b, int count)
{
    printf("%i bytes: ", count);
    int i;
    for (i = 0; i < count; i++) {
	printf("%02x ", b[i]);
    }
    printf("\n");
}

void dump_data_ascii(unsigned char *b, int count)
{
    int i;
    for (i = 0; i < count; i++) {
	printf("%c", b[i]);
    }
}


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
    return 0;
}


int main()
{
    int count, i;

    initi2c();

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    initserialport();

    while (1) {
	count = readSerialString();
	buf[count - 1] = '\r';
	buf[count++] = '\n';
//        dump_data((unsigned char *) buf, count); 
//        printf("Count %d  %s",count, buf);
	process_nmea(buf, count);
	for (i = 0; i < 200; i++)
	    buf[i] = '\0';
    }

    nmea_parser_destroy(&parser);

    close(i2cfd);
    close(fd);
    return 0;
}
