#Author: Michiel van der Coelen
#date: 2011-5-5
#tabsize: 4

NAME = main
MAPLE = /home/michiel/maple13/
LD_LIBRARY_PATH = $(MAPLE)bin.X86_64_LINUX/
CFLAGS = -I$(MAPLE)extern/include/ -Isrc 
CPPFLAGS = $(CFLAGS) -std=c++0x  
USBLIBS := $(shell libusb-config --libs)
LIBS =  -L$(LD_LIBRARY_PATH) -lhf -lrt -lprocessor64 -lmaple -lmaplec $(USBLIBS) -lpthread 
COMPILER = g++
CC = gcc
OUTPUTNAME = $(NAME) 
OBJECTS = opendevice.o $(NAME).o


.PHONY:all, clean, force

all:bin/$(OUTPUTNAME)

force:clean all

bin/$(OUTPUTNAME):$(OBJECTS)
	$(COMPILER) -o bin/$(OUTPUTNAME) $(OBJECTS) $(LIBS)
clean:
	rm bin/$(OUTPUTNAME)
	rm $(OBJECTS)

%.o:src/%.cpp
	$(COMPILER) $(CPPFLAGS) -c $<

%.o:src/%.c
	$(COMPILER) $(CPPFLAGS) -c $<
