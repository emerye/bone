/*
 * font.cpp
 *
 *  Created on: 30 Jan 2017
 *      Author: root
 */

#include <stdio.h>
#include <inttypes.h>
#include "gfxfont.h"
#include "FreeMono12pt7b.h"
//#include "Fonts/FreeSerif9pt7b.h"



/*
void fontInfo(const char *fontFileName) {
	   c -= pgm_read_byte(&gfxFont->first);
	    GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
	    uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

	    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
	    uint8_t  w  = pgm_read_byte(&glyph->width),
	             h  = pgm_read_byte(&glyph->height),
	             xa = pgm_read_byte(&glyph->xAdvance);
	    int8_t   xo = pgm_read_byte(&glyph->xOffset),
	             yo = pgm_read_byte(&glyph->yOffset);
	    uint8_t  xx, yy, bits, bit = 0;
	    int16_t  xo16, yo16;

	    if(size > 1) {
	      xo16 = xo;
	      yo16 = yo;
	    }
}
*/
void printChar(GFXglyph glyph) {
	printf("GFXGlyph width %d height %d yAdvance %d\n",
				glyph.width, glyph.height, glyph.xAdvance);
	printf("Bitmap offset %d\n",glyph.bitmapOffset);

}


void fontTest(GFXfont font)
{
	return;
	/*
	typedef struct { // Data stored PER GLYPH
		uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
		uint8_t  width, height;    // Bitmap dimensions in pixels
		uint8_t  xAdvance;         // Distance to advance cursor (x axis)
		int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
	} GFXglyph;

	typedef struct { // Data stored for FONT AS A WHOLE:
		uint8_t  *bitmap;      // Glyph bitmaps, concatenated
		GFXglyph *glyph;       // Glyph array
		uint8_t   first, last; // ASCII extents
		uint8_t   yAdvance;    // Newline distance (y axis)
	} GFXfont;
	*/

	int i;

	printf("Font first char %d\n", font.first);
	printf("Font last char %d\n", font.last);
	printf("Font yAdvance %d\n", font.yAdvance);

	printf("GFXGlyph width %d height %d yAdvance %d\n",
			font.glyph[0].width, font.glyph[0].width, font.glyph[0].xAdvance);

    printChar(font.glyph[2]);
    for (i=0; i<10; i++) {
      printf("Font char 0x%02x\n",font.bitmap[i]);
    }

}


