PWD = $(shell pwd)
KVERSION = $(shell uname -r)
K_DIR = /usr/src/linux-headers-$(KVERSION)/

MODULE_NAME = charDevice
obj-m = charDevice.o

all:
	make -C $(K_DIR) M=$(PWD) modules
clean:
	make -C $(K_DIR) M=$(PWD) clean
	
