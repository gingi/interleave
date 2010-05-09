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
	./fastqinterleave test.fastq tmp.fastq
	diff tmp.fastq expected.fastq
	rm tmp.fastq
	
debug: clean
	$(MAKE) $(MAKEFILE) CFLAGS="$(CFLAGS) -g -D DEBUG"