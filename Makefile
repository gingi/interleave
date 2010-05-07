CC=gcc 
CFLAGS=-Wall
EXECDIR=$(HOME)/bin

fastqinterleave: fastqinterleave.o

clean:
	rm -f fastqinterleave{,.o}

install:
	cp fastqinterleave $(EXECDIR)