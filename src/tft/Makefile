CC=gcc
CFLAGS = -Wall -g
OBJECTS = tft.o mmapgpio.o spi595.o ssd1963.o  
FILES = tft.c spi595.c mmapgpio.c ssd1963.c 

tft: $(OBJECTS) 
	$(CC) $(CFLAGS) -o tft $(FILES) 

spi595.o: spi595.c tft.h
	$(CC) $(CFLAGS) -o spi595.o -c spi595.c 

mmapgpio.o: mmapgpio.c beaglebone_gpio.h tft.h
	$(CC) $(CFLAGS) -o mmapgpio.o -c mmapgpio.c 

tft.o: tft.c tft.h 
	$(CC) $(CFLAGS) -o tft.o -c tft.c 

ssd1963.o: ssd1963.c tft.h 
	$(CC) $(CFLAGS) -o ssd1963.o -c ssd1963.c 

clean:
	 rm -f tft tft.o mmapgpio.o spi595.o ssd1963.o  
