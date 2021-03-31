#!/bin/bash

scp -r /mnt/c/GoingBeyondArduino/BlinkTimer pi@192.168.0.121:~/GoingBeyondArduino
ssh pi@192.168.0.121 'cd ~/GoingBeyondArduino/BlinkTimer; GB --load'
