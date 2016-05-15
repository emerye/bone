#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "spi595.h" 
#include "gpio.h"

#define RESETLINE 51
#define DCLINE 40

int spiFD;   



int main(int argc, char **argv)
{
    gpio_export(RESETLINE);
    gpio_export(DCLINE);
    gpio_set_dir(DCLINE, 1);
    gpio_set_dir(RESETLINE, 1);


    while (1) {
	gpio_set_value(RESETLINE, 1);
	gpio_set_value(RESETLINE, 0);
	gpio_set_value(DCLINE, 1);
	gpio_set_value(DCLINE, 0);
    }
}
