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
	./interleave -l 4 -o tmp.txt test/left.txt test/right.txt
	diff -u tmp.txt test/expected.2way.txt
	rm tmp.txt
	./interleave -l 1 -o tmp.txt test/{a,b,c}.txt
	diff -u tmp.txt test/expected.3way.txt
	
debug: clean
	$(MAKE) $(MAKEFILE) CFLAGS="$(CFLAGS) -g -D DEBUG"