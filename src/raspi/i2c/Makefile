CC=g++
CFLAGS=-Wall -g 
OBJECTS = TMP100.o    

TMP100:   $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -lwiringPi -o TMP100  

oled.o: oled.cpp 
	$(CC) $(CFLAGS) -c TMP100.cpp -lwiringPi 

clean:
	 $(RM) TMP100 $(OBJECTS) *.o  
