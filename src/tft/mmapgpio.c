#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include "beaglebone_gpio.h"
#include "tft.h"

#define GPIO48 0x00010000
#define GPIO50 0x00040000
#define GPIO51 0x00080000
#define GPIO60 0x10000000
#define RSCMD  GPIO50
#define CS	   GPIO48
#define WRITE  GPIO51
#define RESET  GPIO60

#define HIGH 1
#define LOW 0

typedef enum
{
  LCDCS,
  LCDRS,
  LCDWRITE,
  LCDRESET,
} CTRLLINES;

/*
int
SetPin (void *gpio_addr, CTRLLINES pinNames, int newState)
{
  unsigned int *gpio_setdataout_addr = NULL;
  unsigned int *gpio_cleardataout_addr = NULL;
  unsigned int outData; 
  unsigned int gpioBaseAddr; 

  gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
  gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

  switch (pinNames) 
{
	case LCDCS:
	  outData = GPIO48;
	  break;
    case LCDRS:
      outData = GPIO50;
      break; 
    case LCDWRITE:
      outData = GPIO51;
      break; 
    case LCDRESET:
	  outData = GPIO60;  
      break; 
}	   
	if(newState == 1) {
      *gpio_setdataout_addr = outData;
     }
     else {
       *gpio_cleardataout_addr = outData;
     }
return 0; 
}
*/

int
MapGPIO ()
{
  //void *gpio_addr = NULL;
  volatile unsigned int *gpio_oe_addr = NULL;
  unsigned int reg;
  volatile int setData;

  mmapFD = open ("/dev/mem", O_RDWR);
  if (mmapFD < 0)
    {
      printf ("Error opening map file.\n");
    }
  gpio_addr = mmap (0, GPIO1_SIZE, PROT_READ | PROT_WRITE,
		    MAP_SHARED, mmapFD, GPIO1_START_ADDR);

  gpio_oe_addr = gpio_addr + GPIO_OE;

  if (gpio_addr == MAP_FAILED)
    {
      printf ("Unable to map GPIO\n");
      exit (1);
    }

  reg = *gpio_oe_addr;

  setData = (GPIO48 + GPIO50 + GPIO51 + GPIO60);

  //Configures registers for output
  //Y  reg = reg & (0xFFFFFFFF - (PIN + 1<<18);
  reg = reg & (0xFFFFFFFF - setData);
  *gpio_oe_addr = reg;
  return 0;
}


int
SendCommand (CMDTYPE cmdType)
{
  unsigned int *gpio_setdataout_addr = NULL;
  unsigned int *gpio_cleardataout_addr = NULL;

  gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
  gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

  switch (cmdType)
    {
    case WRITECMD:
      *gpio_cleardataout_addr = GPIO50;	//Command low
      *gpio_cleardataout_addr = GPIO51;	//Write low
      break;

    case WRITEDATA:
      *gpio_setdataout_addr = GPIO50;	//Command high
      *gpio_cleardataout_addr = GPIO51;	//Write low
      break;
    }
  *gpio_setdataout_addr = GPIO51;	//Write high   
  return 0;
}


void
SendDisplayReset()
{
  unsigned int *gpio_setdataout_addr = NULL;
  unsigned int *gpio_cleardataout_addr = NULL;

  gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
  gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

  *gpio_setdataout_addr = GPIO60;	//Set line. 
  usleep(1000 * 15);  
  *gpio_cleardataout_addr = GPIO60;	//Set line low
  usleep(1000 * 25);  
  *gpio_setdataout_addr = GPIO60;	//Set line high and remain. 
  usleep(1000 * 25);  
}




/*
//Send commands. 
int
mmapGPIO(CMDTYPE cmd)  
//main (int argc, char *argv[])
{
  void *gpio_addr = NULL;
  volatile unsigned int *gpio_oe_addr = NULL;
  volatile unsigned int *gpio_setdataout_addr = NULL;
  volatile unsigned int *gpio_cleardataout_addr = NULL;
  unsigned int reg;
  volatile int setData;

  int fd = open ("/dev/mem", O_RDWR);

  gpio_addr = mmap (0, GPIO1_SIZE, PROT_READ | PROT_WRITE,
		    MAP_SHARED, fd, GPIO1_START_ADDR);

  gpio_oe_addr = gpio_addr + GPIO_OE;
  gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
  gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

  if (gpio_addr == MAP_FAILED)
    {
      printf ("Unable to map GPIO\n");
      exit (1);
    }

  reg = *gpio_oe_addr;

  setData = (GPIO48 + GPIO50 + GPIO51 + GPIO60);

  //Configures registers for output
  //Y  reg = reg & (0xFFFFFFFF - (PIN + 1<<18);
  reg = reg & (0xFFFFFFFF - setData);
  *gpio_oe_addr = reg;
  SendCommand(cmd);
  close (fd);
  return 0;
}
*/
