obj-m+=sdb.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
test:
	make -C /scratch/spring2018/group36/linux-yocto-3.19/ M=$(PWD) modules
