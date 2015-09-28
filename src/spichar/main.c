#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include "spictl.h"
#include "lcdchar.h"
#include "tmp100.h"

#define TMP100ADDR 0x48
#define SPI_PATH "/dev/spidev1.0"


int
main ()
{
  int iretVal,i;
  unsigned char cReceiveData[50];
  char tempReading[50]; 
 

  iretVal = initSPI();
  if (iretVal) {
    printf("Not able to open SPI device. Program will exit.\n");
    return -1;
   }

  iretVal = initTMP100(TMP100ADDR);
  if (iretVal) {
    printf("Not able to open I2C device. Program will exit.\n");
    return -1;
   }

 memset (cReceiveData, 0, 50);


 Setup4bit();

 WriteString(0,0,"Wanda Hughes is");
 WriteString(1,0,"missing Thunder");

 for (i=0; i<10; i++) 
{
 iretVal = ReadTemperature(tempReading); 
 if(iretVal < 0) {
   puts("Failed to read temperature.\n"); 
  } else {
  printf("Temperature reading is %s\n",tempReading); 
  }
}
  close (i2cfd);    //Close i2c
  close (SPIfd);    //close spi 
  return 0;
}

