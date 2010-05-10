CC=gcc 
CFLAGS=-Wall
EXECDIR=$(HOME)/bin
OBJS=interleave.o

interleave: $(OBJS)

clean:
	rm -f interleave $(OBJS)

install:
	cp interleave $(EXECDIR)
	
test: interleave
	./interleave -l 4 -o tmp.txt test_left.txt test_right.txt
	diff tmp.txt expected.fastq
	rm tmp.txt
	
debug: clean
	$(MAKE) $(MAKEFILE) CFLAGS="$(CFLAGS) -g -D DEBUG"