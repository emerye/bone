/* PRUSS program to drive a HC-SR04 sensor and display the sensor output
*  in Linux userspace by sending an interrupt.
*  written by Derek Molloy for the book Exploring BeagleBone
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include "lcdchar.h"

#define PRU_NUM 0

static void *pru0DataMemory;
static unsigned int *pru0DataMemory_int;
static int gi2cfd; 

int SetupI2C(int i2cAddr)  
{
  int r;
  int i2cfd;
  char *dev = "/dev/i2c-1";

  i2cfd = open (dev, O_RDWR);
  if (i2cfd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }
  r = ioctl (i2cfd, I2C_SLAVE, i2cAddr);
  if (r < 0)
    perror ("Selecting i2c device.\n");

  Setup4bit(i2cfd);
  DisplayClear(i2cfd);

  WriteString (i2cfd, 0,0, "Started");
  gi2cfd = i2cfd; 
  return i2cfd;  
}



void *threadFunction(void *value){
   char sBuffer[50]; 
   do {
      prussdrv_pru_wait_event (PRU_EVTOUT_1);
      unsigned int raw_distance = *(pru0DataMemory_int+2);
      float distin = ((float)raw_distance / (100 * 148));
      float distcm = ((float)raw_distance / (100 * 58));
      printf("Distance is %f inches (%f cm)             \r", distin, distcm);
      if (distin >= 100.0) {
      sprintf(sBuffer, "%5.2f", distin);
      } else if (distin >= 10.0) {
      sprintf(sBuffer, " %4.2f", distin);
      } else if (distin < 10.0) {
      sprintf(sBuffer, "  %1.2f", distin);
      }

      WriteString(gi2cfd, 1,0, sBuffer); 
      
      prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU0_ARM_INTERRUPT);

   } while (1);
}

int  main (int argc, char **argv)
{
   char sBuffer[50]; 

   if(getuid()!=0){
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }
    
   SetupI2C(0x27); 

   pthread_t thread;
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // Allocate and initialize memory
   prussdrv_init ();
   prussdrv_open (PRU_EVTOUT_0);
   prussdrv_open (PRU_EVTOUT_1);

   // Map PRU's INTC
   prussdrv_pruintc_init(&pruss_intc_initdata);

   // Copy data to PRU memory - different way
   prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pru0DataMemory);
   pru0DataMemory_int = (unsigned int *) pru0DataMemory;
   // Use the first 4 bytes for the number of samples
   *pru0DataMemory_int = 500;
   // Use the second 4 bytes for the sample delay in ms
   *(pru0DataMemory_int+1) = 100;   // 2 milli seconds between samples

   // Load and execute binary on PRU
   prussdrv_exec_program (PRU_NUM, "./ultrasonic.bin");
   if(pthread_create(&thread, NULL, &threadFunction, NULL)){
       printf("Failed to create thread!");
   }
   int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
   printf("PRU program completed, event number %d.\n", n);
   printf("The data that is in memory is:\n");
   printf("- the number of samples used is %d.\n", *pru0DataMemory_int);
   printf("- the time delay used is %d.\n", *(pru0DataMemory_int+1));
   unsigned int raw_distance = *(pru0DataMemory_int+2);
   printf("- the last distance sample is %d.\n", raw_distance);

   // raw_distance is in 10ns samples
   // distance in inches = time (ms) / 148 according to datasheet
   float distin = ((float)raw_distance / (100 * 148));
   float distcm = ((float)raw_distance / (100 * 58));
   printf("-- A distance of %f inches (%f cm).\n", distin, distcm);
   sprintf(sBuffer, "Distance %.3f inches", distin);

   /* Disable PRU and close memory mappings */
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
   return EXIT_SUCCESS;
}
