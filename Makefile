CC=gcc 
CFLAGS=-Wall
EXECDIR=$(HOME)/bin
OBJS=fastqinterleave.o

fastqinterleave: $(OBJS)

clean:
	rm -f fastqinterleave{,.o}

install:
	cp fastqinterleave $(EXECDIR)
	
test: fastqinterleave
	./fastqinterleave -l 4 -o tmp.txt test_left.txt test_right.txt
	diff tmp.txt expected.fastq
	rm tmp.txt
	
debug: clean
	$(MAKE) $(MAKEFILE) CFLAGS="$(CFLAGS) -g -D DEBUG"