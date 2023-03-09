obj-m := vdrm.o
vdrm-objs := vdrm-main.o controller.o vdrm_driver.o vdrm_device.o vdrm_pipe.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean