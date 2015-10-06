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

#define PRU_NUM 0

void LEDState (int State);

char *gpioExport = "/sys/class/gpio/export";

static void *pru0DataMemory;
static unsigned int *pru0DataMemory_int;

//Sonic thread
void *
threadFunction (void *value)
{
  char tempBuffer[20];
  int ledState = 0;
  int retValue;

  do
    {
      usleep(20000); 
      int notimes = prussdrv_pru_wait_event (PRU_EVTOUT_1);
      unsigned int raw_distance = *(pru0DataMemory_int + 2);
      float distin = ((float) raw_distance / (100 * 148));
      float distcm = ((float) raw_distance / (100 * 58));
      printf ("Distance is %f inches (%f cm)             \r", distin, distcm);
      sprintf (tempBuffer, "%5.2f ", distin);
      WriteString (1, 0, tempBuffer);
      if (ledState > 0)
	{
	  retValue = system ("echo 1 > /sys/class/gpio/gpio49/value");
	  ledState = 0;
	}
      else
	{
	  retValue = system ("echo 0 > /sys/class/gpio/gpio49/value");
          ledState += 1; 
	}

      prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU0_ARM_INTERRUPT);
      
    }
  while (1);
}


void
initSonic ()
{
  if (getuid () != 0)
    {
      printf ("You must run this program as root. Exiting.\n");
      exit (EXIT_FAILURE);
    }
  pthread_t thread;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  // Allocate and initialize memory
  prussdrv_init ();
  prussdrv_open (PRU_EVTOUT_0);
  prussdrv_open (PRU_EVTOUT_1);

  // Map PRU's INTC
  prussdrv_pruintc_init (&pruss_intc_initdata);

  // Copy data to PRU memory - different way
  prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, &pru0DataMemory);
  pru0DataMemory_int = (unsigned int *) pru0DataMemory;
  // Use the first 4 bytes for the number of samples
  *pru0DataMemory_int = 100000;
  // Use the second 4 bytes for the sample delay in ms
  *(pru0DataMemory_int + 1) = 2000;	// 2 milli seconds between samples

  // Load and execute binary on PRU
  prussdrv_exec_program (PRU_NUM, "./ultrasonic.bin");
  if (pthread_create (&thread, NULL, &threadFunction, NULL))
    {
      printf ("Failed to create thread!");
    }
  int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
  printf ("PRU program completed, event number %d.\n", n);
  printf ("The data that is in memory is:\n");
  printf ("- the number of samples used is %d.\n", *pru0DataMemory_int);
  printf ("- the time delay used is %d.\n", *(pru0DataMemory_int + 1));
  unsigned int raw_distance = *(pru0DataMemory_int + 2);
  printf ("- the last distance sample is %d.\n", raw_distance);

  // raw_distance is in 10ns samples
  // distance in inches = time (ms) / 148 according to datasheet
}


void
SetupGPIO ()
{
  system ("echo 49 > /sys/class/gpio/export");
}

void
LEDState (int ledState)
{
  if (ledState == 1)
    {
      system ("echo 1 > /sys/class/gpio/gpio49/value");
    }
  else
    {
      system ("echo 0 > /sys/class/gpio/gpio49/value");
    }
}


int
main ()
{
  time_t rawTime;
  int iretVal;
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

  time (&rawTime);
  timeinfo = localtime (&rawTime);
  strftime (date, 50, "%c", timeinfo);
  strftime (date, 50, "%r", timeinfo);
  WriteString (0, 0, date);

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

  initSonic ();

  close (i2cfd);		//Close i2c
  close (SPIfd);		//close spi 
  return 0;
}
