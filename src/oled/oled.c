#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "spi.h"
#include "gpio.h"

#define RESETLINE 51
#define DCLINE 50

enum cmd { COMMAND, DATA };

int spiFD;


int SendByte(enum cmd cmdType, int data)
{
    int error;

    if (cmdType == COMMAND) {
	gpio_set_value(DCLINE, 0);
        puts("Command"); 
    } else {
	gpio_set_value(DCLINE, 1);
        puts("Data Command"); 
    }
    error = SPISendByte(spiFD, data);
    if (error == -1) {
	printf("Error %d\n", error);
    }
    return error;
}


void initDisplay()
{
    gpio_set_value(RESETLINE, 1);
    gpio_set_value(RESETLINE, 0);
    usleep(1000);
    gpio_set_value(RESETLINE, 1);

    sleep(1);

    SendByte(COMMAND, 0xFD);	// Set Command Lock
    SendByte(COMMAND, 0x12);	//   Default => 0x12
    //     0x12 => Driver IC interface is unlocked from entering command.
    //     0x16 => All Commands are locked except 0xFD.

    SendByte(COMMAND, 0xae);	//--turn off oled panel

    SendByte(COMMAND, 0xD5);	// Set Display Clock Divide Ratio / Oscillator Frequency
    SendByte(COMMAND, 0xa0);	//   Default => 0x70
    //     D[3:0] => Display Clock Divider
    //     D[7:4] => Oscillator Frequency

    SendByte(COMMAND, 0xa8);	//--set multiplex ratio(1 to 64)
    SendByte(COMMAND, 0x3f);	//--1/64 duty

    SendByte(COMMAND, 0xd3);	//Set Display Offset
    SendByte(COMMAND, 0x00);

    SendByte(COMMAND, 0x40);	// Set Display Start Line

    SendByte(COMMAND, 0x20);	// Set Memory Addressing Mode
    SendByte(COMMAND, 0x02);	//   Default => 0x02
    //     0x00 => Horizontal Addressing Mode
    //     0x01 => Vertical Addressing Mode
    //     0x02 => Page Addressing Mode


    SendByte(COMMAND, 0xa1);	//--set segment re-map 128 to 0

    SendByte(COMMAND, 0xC8);	//--Set COM Output Scan Direction 64 to 1


    SendByte(COMMAND, 0xda);	//--Set COM Pins Hardware Configuration
    SendByte(COMMAND, 0x12);	//     Disable COM Left/Right Re-Map   Alternative COM Pin Configuration


//    SendByte(COMMAND, 0x81);	//--set contrast control register


    //     SPISendByte(Contrast_level);


    SendByte(COMMAND, 0xD9);	// Set Pre-Charge Period
    SendByte(COMMAND, 0x25);	//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
    //     D[3:0] => Phase 1 Period in 1~15 Display Clocks
    //     D[7:4] => Phase 2 Period in 1~15 Display Clocks

    SendByte(COMMAND, 0xDB);	// Set VCOMH Deselect Level
    SendByte(COMMAND, 0x34);	//   Default => 0x34 (0.78*VCC)


    SendByte(COMMAND, 0xad);	//--Set Master Configuration
    SendByte(COMMAND, 0x8e);	//--

    SendByte(COMMAND, 0xd8);	//--Set Area Color Mode On/Off & Low Power Display Mode
    SendByte(COMMAND, 0x05);	//


    SendByte(COMMAND, 0xa6);	//--set normal display

    SendByte(COMMAND, 0xa4);	//Disable Entire Display On     

    SendByte(COMMAND, 0xaf);	//--turn on oled panel
}


void init_Hardware(void)
{
    gpio_export(RESETLINE);
    gpio_export(DCLINE);
    gpio_set_dir(DCLINE, 1);
    gpio_set_dir(RESETLINE, 1);
}


int main(int argc, char **argv)
{
    init_Hardware();
    Init_SPI();
    initDisplay();
    sleep(1);
    SendByte(DATA, 0xFF); 
    while(1) {
    }
    return 0;
}
