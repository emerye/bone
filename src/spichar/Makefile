CC=gcc
CFLAGS = -Wall -g
OBJECTS = spictl.o lcdchar.o tmp100.o main.o 
FILES = spictl.c lcdchar.c tmp100.c main.c 

spictl: $(OBJECTS) 
	$(CC) $(CFLAGS) -o spictl $(FILES) 

spictl.o: spictl.c spictl.h 
	$(CC) $(CFLAGS) -o spictl.o -c spictl.c 

lcdchar.o: lcdchar.c lcdchar.h 
	$(CC) $(CFLAGS) -o lcdchar.o -c lcdchar.c 

tmp100.o: tmp100.c tmp100.h 
	$(CC) $(CFLAGS) -o tmp100.o -c tmp100.c 

main.o: main.c 
	$(CC) $(CFLAGS) -o main.o -c main.c 

clean:
	 rm -f spictl spictl.o lcdchar.o tmp100.0 main.o 
