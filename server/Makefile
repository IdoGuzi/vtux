CC=gcc
CFLAGS = -Wall -g
INCDIR:=-I$(PWD)/../common
LIBDIR:=-L$(PWD)/../lib

server: main.c
	$(CC) $(CFLAGS) -o server main.c $(INCDIR) $(LIBDIR) -lcommon

.PHONY: clean

clean:
	rm -rf *.o server