CC=gcc
CFLAGS=-I. -Wall -c

memtest: memtest.o help.o
	$(CC) -o  memtest memtest.o help.o
memtest.o: memtest.c
	$(CC) $(CFLAGS) memtest.c
help.o: help.c
	$(CC) $(CFLAGS) help.c
clean:
	rm -rf *o memtest