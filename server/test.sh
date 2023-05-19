#!/bin/bash

make clean && make
sleep 5s
echo runnig server >> log.txt
while :;
do
	./server >> log.txt
	dmesg -c >> "$(date +"%H:%M")"
	ret=$?
	if [ $ret -eq 0 ];
	then
		break
	fi
done