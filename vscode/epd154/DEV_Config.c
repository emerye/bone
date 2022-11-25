/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
****************************************************/
#define OUTPUT 1
#define INPUT 0

#include "DEV_Config.h"
#include <errno.h>


void DEV_SPI_WriteByte(UBYTE value)
{
    int read_data;
    read_data = spi_write_blocking(spi0, &value, 1);
    if(read_data < 1)
        perror("SPI write failed.\r\n");
}


/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_ModuleExit(void)
{

}