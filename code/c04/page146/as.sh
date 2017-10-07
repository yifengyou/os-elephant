#!/bin/bash 
nasm -f bin 16push.S -o 16push.o
objdump -D -b binary -m i8086 16push.o
