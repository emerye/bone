
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <signal.h>     //signal()
#include <time.h>
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "ER-EPD0154-2B.h"
#include "hardware/spi.h"
#include "Fonts/fonts.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    EPD_Sleep();
    DEV_ModuleExit();
    exit(0);
}

void InitPicoSPI(int channel) {

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*2000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);

    //Pin Data/Command
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_put(PIN_DC, 1);
    gpio_set_function(PIN_DC, GPIO_FUNC_SIO);

    //Pin Reset
    gpio_set_function(PIN_RESET, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_put(PIN_RESET, 1);

    //Busy input pin
    gpio_set_function(PIN_BUSY, GPIO_FUNC_SIO);
    gpio_set_dir(PIN_BUSY, GPIO_IN);
}


int main(void)
{
    int i;

    stdio_init_all();
    printf("1.54inch e-Paper demo\r\n");
    printf("Version 0.2\n");
    InitPicoSPI(0);

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if(EPD_Init() != 0) {
      printf("e-Paper init failed\r\n");
    }
    printf("epd  clear\r\n");
    EPD_Clear();

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
  
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 0, BLACK);
    Paint_Clear(WHITE);

    Paint_DrawString_EN(1, 1, "01", &Font160, WHITE, BLACK);
    EPD_Display(BlackImage);

    /*show image for array*/

    printf("show image for array\r\n");
    Paint_Clear(WHITE);
    printf("Drawing Bitmap from gimage.\n");
    Paint_DrawBitMap(gImage);
    EPD_Display(BlackImage);
    DEV_Delay_ms(1000);



#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    EPD_Clear();
   

    // 2.Drawing on the image
   
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawLine(70, 70, 20, 120, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);    
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawCircle(45, 95, 20, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawCircle(105, 95, 20, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawLine(85, 95, 125, 95, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawLine(105, 75, 105, 115, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawString_EN(0, 0, "www.buydisplay.com", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
 //   Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
 //   Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
 
   // Paint_DrawNum(1, 1, 0, &Font64, WHITE, BLACK);
  //  Paint_DrawNum(1, 90, 56789, &Font64, WHITE, BLACK);
    EPD_Display(BlackImage);
  
    Paint_Clear(WHITE);
    EPD_Clear();
    Paint_DrawString_EN(100, 0, "0", &Font160, WHITE, BLACK);
    EPD_Display(BlackImage);
    Paint_Clear(WHITE);
    sleep_ms(1000);
    
    for(int i=10; i<20; i++) {
      Paint_Clear(WHITE);
      Paint_DrawNum(1, 1, i, &Font160, WHITE, BLACK);
      EPD_Display(BlackImage);
      sleep_ms(1000);
    }

    EPD_Display(BlackImage);
    DEV_Delay_ms(5000);
#endif

    printf("epd  clear------------------------\r\n");
    EPD_Clear();

    printf("Goto Sleep mode...\r\n");
    EPD_Sleep();
    free(BlackImage);
    return 0;
}
