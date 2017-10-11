#!/bin/bash 
nasm -o header.bin header.S

./xxd.sh header.bin 0 300
