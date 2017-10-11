#!/bin/bash 
gcc -c -m32 -o entry.o main.c
ld -melf_i386 -Ttext 0xc000500 -e main -o entry.bin entry.o
file entry.bin
nm entry.bin
readelf -h entry.bin 
./xxd.sh entry.bin 1280 40
