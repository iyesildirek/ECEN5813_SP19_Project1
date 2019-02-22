CC=gcc
CFLAGS=-c -g -Wall -Werror

memtest: memtest.o help.o allocate.o free.o
	$(CC) -o  memtest memtest.o help.o allocate.o free.o
	$(CC) $(CFLAGS) memtest.c
help.o: help.c
	$(CC) $(CFLAGS) help.c
allocate.o:
	$(CC) $(CFLAGS) allocate.c
free.o: free.c
	$(CC) $(CFLAGS) free.c
clean:
	rm -rf *o memtest
