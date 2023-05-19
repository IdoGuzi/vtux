#!/bin/bash

(cd driver; sudo ./test.sh) &
(cd server; ./test.sh) &
sleep 20s
reboot