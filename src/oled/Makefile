CC=gcc
CFLAGS=-Wall -g 
OBJECTS = oled.o    

oled:   $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -lwiringPi -o oled  

oled.o: oled.c 
	$(CC) $(CFLAGS) -c oled.c -lwiringPi 

main.o:	main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c 

clean:
	 $(RM) oled  $(OBJECTS) *.o  
