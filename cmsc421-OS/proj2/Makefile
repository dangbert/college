# Basic Makefile to pull in kernel's KBuild to build an out-of-tree
# kernel module

KDIR ?= /lib/modules/$(shell uname -r)/build
MODNAME = pwkeeper

all: modules $(MODNAME)-test

$(MODNAME)-test: $(MODNAME)-test.o cs421net.o
	gcc --std=c99 -Wall -O2 -pthread -o $@ $^ -lm

$(MODNAME)-test.o: $(MODNAME)-test.c cs421net.h
cs421net.o: cs421net.c cs421net.h

%.o: %.c
	gcc --std=c99 -Wall -O2 -c -o $@ $<

modules xt_cs421net.ko:
	$(MAKE) -C $(KDIR) M=$$PWD $@

unit-test: pwkeeper-test.c
	gcc --std=c99 -Wall -O2 -pthread -o $@ $^ -lm

clean:
	$(MAKE) -C $(KDIR) M=$$PWD $@
	-rm $(MODNAME)-test

unload:
	sudo rmmod pwkeeper
load:
	sudo insmod pwkeeper.ko
	lsmod | grep -E 'pwkeeper|xt_cs421net'
	ls -l /dev/pwkeeper*

prep:
	make && make unload && make load

log:
	dmesg -T | tail
	ls -l /dev/pwkeeper*

test:
	make unit-test && ./unit-test

# view ID: id -u $USER
# test with:
# echo -n 'password7' > /dev/pwkeeper_master
# (note: doing these echos from the Makefile doesn't work for some reason)
# created users test1, ... test4
# adduser <username>
# sudo su <username>

.PHONY: all modules clean unload load prep log test
