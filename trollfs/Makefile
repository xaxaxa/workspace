obj-m := trollfs.o
trollfs-objs := main.o
#KERNEL=$(shell uname -r)
KERNEL=3.2.0-29-generic-pae
all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean

