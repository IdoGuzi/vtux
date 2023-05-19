CC=gcc
AR=ar rc
CFLAGS=-Wall -g
OBJECTS= common.o
LIBDIR=$(PWD)/../lib
TARGET=$(LIBDIR)/libcommon.a

all: directory $(OBJECTS) $(TARGET) clean_objects

$(TARGET): $(OBJECTS)
	$(AR) $(TARGET) $(OBJECTS)

$(OBJECTS): vdrm_ioctl.c vdrm_ioctl.h
	$(CC) $(CFLAGS) -o $(OBJECTS) -c vdrm_ioctl.c

directory:
	mkdir -p $(LIBDIR)

clean_objects:
	rm -rf $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET)