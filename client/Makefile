CC=gcc
OLIB=lib
OOBJ=obj
COMPOSITOR=$(XDG_SESSION_TYPE)
CFLAGS=-Wall -g -std=c99
DFLAGS=-D compositor=$(COMPOSITOR)
XFLAGS=-lX11 -lxcb -lxcb-randr 
DRMFLAGS=-ldrm -I/usr/include/libdrm
DRM_INC=-I/usr/include/libdrm
DRM_LIB=-ldrm
INCDIR:=-I$(PWD)/../common
LIBDIR:=-L$(PWD)/../lib


vtux: main.c client.o
	$(CC) $(CFLAGS) $(DRM_INC) -o vtux $(OOBJ)/*.o main.c $(DRM_LIB)

client.o: $(OOBJ) client.c client.h
	$(CC) $(CFLAGS) -c client.c -o $(OOBJ)/client.o $(INCDIR)

#vtux: main.o libx.so
#	$(CC) $(CFLAGS) -o vtux $(OOBJ)/main.o ./$(OLIB)/libx.so
#
#main.o: out main.c 
#	$(CC) $(CFLAGS) $(DFLAGS) main.c -c -o $(OOBJ)/main.o $(DRMFLAGS)
#
#libx.so: out x.o
#	$(CC) -shared -o $(OLIB)/libx.so $(OOBJ)/x.o $(XFLAGS)
#
#x.o: out x/x.h x/x.c
#	$(CC) -fPIC $(CFLAGS) x/x.c -c -o $(OOBJ)/x.o
#
#out:
#	mkdir -p $(OLIB) $(OOBJ)

$(OOBJ):
	mkdir -p $(OOBJ)

.PHONY: clean

clean:
	rm -rf $(OLIB) $(OOBJ) vtux
