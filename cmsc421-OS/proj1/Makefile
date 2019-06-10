# Basic Makefile to pull in kernel's KBuild to build an out-of-tree
# kernel module

KDIR ?= /lib/modules/$$(uname -r)/build

all: modules sha3-test

sha3-test: sha3-test.c
	gcc --std=c99 -Wall -O2 -pthread -o $@ $^ -lm

modules:
	$(MAKE) -C $(KDIR) M=$$PWD $@

clean:
	$(MAKE) -C $(KDIR) M=$$PWD $@
	-rm sha3-test

unload:
	sudo rmmod sha3
load:
	sudo insmod sha3.ko
	lsmod | grep sha3
	ls -l /dev/sha3*
# dmesg | tail

prep:
	make && make unload && make load

log:
	xxd /dev/sha3_data | head
	xxd /dev/sha3_ctl | head
	dmesg -T | tail

indent:
	~/linux/scripts/Lindent sha3.c

test:
	make
	./sha3-test

# test with:
# echo -n 'CMSC 421' > /dev/sha3_data
# echo -n '8' > /dev/sha3_ctl && make log
# (note: doing these echos from the Makefile doesn't work for some reason)

.PHONY: all modules clean unload load prep log test indent test
