#!/bin/sh
killall nc 2> /dev/null
sudo iptables -t mangle -F INPUT
sudo rmmod xt_cs421net.ko
