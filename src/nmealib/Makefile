CC = gcc -fPIC -g -Wall
 
BIN = lib/libnmea.so 
MODULES = generate generator parse parser tok context time info gmath sentence  
SAMPLES = generate generator parse parse_file math
 
OBJ = $(MODULES:%=build/nmea_gcc/%.o) 
LINKOBJ = $(OBJ) $(RES)

SMPLS = $(SAMPLES:%=samples_%)
SMPLOBJ = $(SAMPLES:%=samples/%/main.o)

INCS = -I include 
LIBS = -lm -Llib -lnmea
 
.PHONY: all all-before all-after clean clean-custom doc
 
all: all-before $(BIN) sharedlib samples all-after  

all-before:
	mkdir -p build/nmea_gcc

clean: clean-custom 
	rm -f $(LINKOBJ) $(BIN) $(SMPLOBJ) $(SMPLS) libnmea.so*

doc:
	$(MAKE) -C doc
	
remake: clean all

install: libnmea.so
	sudo -i cd /home/andy/bone/src/nmealib	
	sudo -i cp /home/andy/bone/src/nmealib/libnmea.so /usr/local/lib
	sudo ldconfig

$(BIN): $(LINKOBJ)
	ar rsc $@ $^
	ranlib $@

sharedlib: $(LINKOBJ)
	gcc -shared -Wl,-soname,libnmea.so \
	-o libnmea.so $^ -lc  

build/nmea_gcc/%.o: src/%.c 
	$(CC) $(INCS) -c $< -o $@

samples: $(SMPLS)

samples_%: samples/%/main.o
	$(CC) $< $(LIBS) -o build/$@

samples/%/main.o: samples/%/main.c
	$(CC) $(INCS) -c $< -o $@
