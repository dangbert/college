#!/bin/sh
set -e
[ -f xt_cs421net.ko ] || make xt_cs421net.ko
sudo modprobe x_tables
sudo insmod xt_cs421net.ko
sudo iptables -t mangle -A INPUT -p tcp --dport 4210 -j LOG
nc -d -k -l 4210 > /dev/null &
