CC=gcc 
CFLAGS=-Wall
EXECDIR=$(HOME)/bin

fastqinterleave: fastqinterleave.o

clean:
	rm -f fastqinterleave{,.o}

install:
	cp fastqinterleave $(EXECDIR)
	
test: fastqinterleave
	./fastqinterleave test.fastq tmp.fastq
	diff tmp.fastq expected.fastq
	rm tmp.fastq