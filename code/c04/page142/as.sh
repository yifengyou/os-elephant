#!/bin/bash 
nasm -f bin 1bits.S -o 1bits.bin
nasm -f bin 2bits.S -o 2bits.bin
objdump -D -m i8086 -b binary  1bits.bin
objdump -D -m i386 -b binary  2bits.bin
