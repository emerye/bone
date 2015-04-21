#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include "beaglebone_gpio.h"

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

typedef enum
{
	WRITECMD,
	WRITEDATA,
} CMDTYPE;

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
SendCommand(void *gpio_addr, CMDTYPE cmdType)
{
  unsigned int *gpio_setdataout_addr = NULL;
  unsigned int *gpio_cleardataout_addr = NULL;

  gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
  gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

  switch (cmdType) 
  {
	case WRITECMD:
      *gpio_cleardataout_addr = GPIO50;    //Command low
      *gpio_cleardataout_addr = GPIO51;    //Write low
      usleep(1); 
	  break;

    case WRITEDATA:
      *gpio_setdataout_addr = GPIO50;    //Command high
      *gpio_cleardataout_addr = GPIO51;    //Write low
      usleep(1);
      break; 
  }	   
   *gpio_setdataout_addr = GPIO51;     //Write high   
   usleep(1); 
return 0; 
}


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

  printf ("Mapping %X - %X (size: %X)\n", GPIO1_START_ADDR,
	  GPIO1_END_ADDR, GPIO1_SIZE);

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
  printf ("GPIO mapped to %p\n", gpio_addr);
  printf ("GPIO OE mapped to %p\n", gpio_oe_addr);
  printf ("GPIO SETDATAOUTADDR mapped to %p\n", gpio_setdataout_addr);
  printf ("GPIO CLEARDATAOUT mapped to %p\n", gpio_cleardataout_addr);

  reg = *gpio_oe_addr;
  printf ("Current GPIO1 configuration: %X\n", reg);

  setData = (GPIO48 + GPIO50 + GPIO51 + GPIO60);

  //Configures registers for output
  //Y  reg = reg & (0xFFFFFFFF - (PIN + 1<<18);
  reg = reg & (0xFFFFFFFF - setData);
  *gpio_oe_addr = reg;
  printf ("GPIO1 new config: %X\n", reg);


  printf ("Start toggling PIN \n");
  while (1)
    {
 //SetPin (gpio_addr, LCDRESET, HIGH);
  SendCommand(gpio_addr, WRITECMD);
  SendCommand(gpio_addr, WRITEDATA); 
/*
     *gpio_setdataout_addr = setData;
    usleep (1);
    *gpio_cleardataout_addr = setData;
  usleep (1);

*/

    }

  close (fd);
  return 0;
}
