#include <stdio.h>
#include <inttypes.h>
#include "../oled1309/oled1309.h"
#include "../oled1309/gfxfont.h"
#include "../oled1309/glcdfont.h"

int main(int argc, char **argv) {

	printf("Started\n");
	oled1309 objRef(1);
	oled1309 obj2(5);

	obj2.print();

    objRef.print();
}
