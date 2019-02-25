CC=gcc
CFLAGS=-c -Wall -Werror

memtest: memtest.o help.o allocate.o free.o display.o write.o invert.o
	$(CC) -o  memtest memtest.o help.o allocate.o free.o display.o write.o invert.o
	$(CC) $(CFLAGS) memtest.c
help.o: help.c
	$(CC) $(CFLAGS) help.c
allocate.o: allocate.c
	$(CC) $(CFLAGS) allocate.c
free.o: free.c
	$(CC) $(CFLAGS) free.c
display.o: display.c
	$(CC) $(CFLAGS) display.c
write.o: write.c
	$(CC) $(CFLAGS) write.c
invert.o: invert.c
	$(CC) $(CFLAGS) invert.c
clean:
	rm -rf *o memtest
