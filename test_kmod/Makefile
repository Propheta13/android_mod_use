obj-m := test_kmod.o

ifndef KMODDIR
KMODDIR = /lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KMODDIR) M=$(shell pwd) modules
	cp -f ./test_kmod.ko ../

help:
	@echo "+========================== USAGE ===========================+"
	@echo "| You may set KMODDIR variable for specific kernel version   |"
	@echo "| I.E. KMODDIR=/lib/modules/2.6.xx-my-minor-kernel make      |"
	@echo "+============================================================+"

clean:
	make -C $(KMODDIR) M=$(shell pwd) clean
