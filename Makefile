#Author: Michiel van der Coelen
#date: 2011-5-3
#tabsize: 4

NAME = main
MAPLE = /home/michiel/maple13/
LD_LIBRARY_PATH = $(MAPLE)bin.X86_64_LINUX/
CFLAGS = -L$(LD_LIBRARY_PATH) -lhf -lrt -lprocessor64 -lmaple -lmaplec -std=c++0x -I$(MAPLE)extern/include/ -Isrc -lpthread #windows: remove pthreads
COMPILER = g++
OUTPUTNAME = $(NAME) #windows: change to exe


.PHONY:all, clean
all: bin/$(OUTPUTNAME)

bin/$(OUTPUTNAME):src/$(NAME).cpp
	$(COMPILER) src/$(NAME).cpp -o bin/$(OUTPUTNAME) $(CFLAGS)

clean: 
	rm bin/$(OUTPUTNAME)

