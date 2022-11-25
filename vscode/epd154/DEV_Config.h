/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
****************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_DC  14  
#define PIN_RESET 15
#define PIN_BUSY 22   //Input

/**
 * GPIO read and write
**/
#define DEV_Digital_Write(_pin, _value) gpio_put(_pin, _value)
#define DEV_Digital_Read(_pin) gpio_get_out_level(_pin)


/**
 * delay x ms
**/
#define DEV_Delay_ms(__xms) sleep_ms(__xms)

/*------------------------------------------------------------------------------------------------------*/
void DEV_SPI_WriteByte(UBYTE value);
void DEV_ModuleExit(void);


#endif
