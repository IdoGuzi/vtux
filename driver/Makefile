obj-m := vdrm.o
KDIR:=/lib/modules/$(shell uname -r)/build
vdrm-objs:=vdrm_main.o \
		controller.o \
		vdrm_driver.o \
		vdrm_device.o \
		vdrm_pipe.o \
		vdrm_ioctl.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean