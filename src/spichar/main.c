#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include <time.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <pthread.h>
#include "spictl.h"
#include "lcdchar.h"
#include "tmp100.h"

#define TMP100ADDR 0x48
#define SPI_PATH "/dev/spidev1.0"

char *gpioExport = "/sys/class/gpio/export";
int ledState;


void
SetupGPIO ()
{
  system ("echo 49 > /sys/class/gpio/export");
}

void
LEDState ()
{
  if (ledState == 1)
    {
      ledState = 0;
      system ("echo 1 > /sys/class/gpio/gpio49/value");
    }
  else
    {
      system ("echo 0 > /sys/class/gpio/gpio49/value");
      ledState = 1;
    }
}


int
main ()
{
  time_t rawTime;
  int iretVal, i;
  char tempReading[50];
  char date[50];
  struct tm *timeinfo;

  iretVal = initSPI ();
  if (iretVal)
    {
      printf ("Not able to open SPI device. Program will exit.\n");
      return -1;
    }

  iretVal = initTMP100 (TMP100ADDR);
  if (iretVal)
    {
      printf ("Not able to open I2C device. Program will exit.\n");
      return -1;
    }

  Setup4bit ();
  SetupGPIO ();

  for (i = 0; i < 1000000; i++)
    {
      time (&rawTime);
      timeinfo = localtime (&rawTime);
      strftime (date, 50, "%c", timeinfo);
      strftime (date, 50, "%r", timeinfo);
      WriteString (0, 0, date);

      printf ("Time: %s\n", date);

      iretVal = ReadTemperature (tempReading);
      if (iretVal < 0)
	{
	  puts ("Failed to read temperature.\n");
	}
      else
	{
	  printf ("Temperature reading is %s\n", tempReading);
	  WriteString (3, 0, "             ");
	  WriteString (3, 0, tempReading);
	}
      sleep (1);
      LEDState ();
    }
  close (i2cfd);		//Close i2c
  close (SPIfd);		//close spi 
  return 0;
}
