CC=gcc
CFLAGS=-Wall -g
OBJECTS = tft.c 

tft: $(OBJRCTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o tft 

%.o : %.c
	$(CC) $(CFLAGS) -o $<

clean:
	 rm -f tft tft.o 
