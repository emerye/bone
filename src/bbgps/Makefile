CC=gcc
CFLAGS=-Wall -Werror
OBJECTS = main.o serial.o 

all:   bbgps


bbgps:  $(OBJECTS)  
	$(CC) $(CFLAGS) -L/usr/local/lib -llcdchar -o bbgps $(OBJECTS)  

i2cdisp.o:  i2cdisp.c 
	$(CC) $(CFLAGS) -c i2cdisp.c 

main.o:	main.c
	$(CC) $(CFLAGS) -c main.c 

serial.o: serial.c
	$(CC) $(CFLAGS) -c serial.c 

clean:
	 $(RM) bbgps i2cdisp.o   
