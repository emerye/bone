/*
 * main.c
 *
 *  Created on: 21 Apr 2017
 *      Author: root
 */

#include <stdio.h>

#define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

int main()
{
    int red,green,blue;
    unsigned short bgr565;
    while (1) {
        red=getchar(); if (red == EOF) return (0);
        green=getchar(); if (green == EOF) return (1);
        blue=getchar(); if (blue == EOF) return (1);
        bgr565 = RGB2COLOR(red, green, blue);
            putchar((bgr565 >> 8) & 0xFF);
            putchar(bgr565 & 0xFF);
        }
    }
