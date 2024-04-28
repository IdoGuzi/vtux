CC=gcc
CFLAGS = -Wall -g
INCDIR:=-I$(PWD)/../common
LIBDIR:=-L$(PWD)/../lib

server: main.c server.o
	$(CC) $(CFLAGS) -o server server.o main.c $(INCDIR) $(LIBDIR) -pthread -lcommon

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c $(INCDIR)

.PHONY: clean

clean:
	rm -rf *.o server
