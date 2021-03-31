#!/bin/bash

rsync -a /mnt/c/GoingBeyondArduino/ pi@192.168.0.121:~/GoingBeyondArduino
ssh pi@192.168.0.121 'cd ~/GoingBeyondArduino/Blink; GB --load'
