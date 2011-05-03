#Author: Michiel van der Coelen
#date: 2011-5-3
#tabsize: 4 spaces

NAME = maple_test
CFLAGS = -Isrc -lpthread #windows: remove pthreads
COMPILER = g++
OUTPUTNAME = $(NAME) #windows: change to exe

.PHONY:all, clean
all: bin/$(OUTPUTNAME)

bin/$(OUTPUTNAME):src/$(NAME).cpp
	$(COMPILER) src/$(NAME).cpp -o bin/$(OUTPUTNAME) $(CFLAGS)

clean: 
	rm bin/$(OUTPUTNAME)

