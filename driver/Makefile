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

compile_commands:
	make -C $(KDIR) M=$(PWD) compile_commands.json

clean:
	cp compile_commands.json compile_commands.json.tmp
	make -C $(KDIR) M=$(PWD) clean
	mv compile_commands.json.tmp compile_commands.json
