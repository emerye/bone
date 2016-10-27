CC=g++
CFLAGS=-Wall -g 
OBJECTS = oled.o    

oled:   $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -lwiringPi -o oled  

oled.o: oled.cpp 
	$(CC) $(CFLAGS) -c oled.cpp -lwiringPi 

clean:
	 $(RM) oled  $(OBJECTS) *.o  
