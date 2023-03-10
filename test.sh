#!/bin/bash

auditctl -e 0
dmesg -c
make clean
make
sleep 2s #sleep so ko will finish compiling
insmod vdrm.ko
sleep 2s
dmesg -c > "$(date +"%H:%M")"
sleep 10s
reboot