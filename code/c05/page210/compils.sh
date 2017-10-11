#!/bin/bash 
gcc -c -o start.o start.c
ld start.o -Ttext 0xc0001500 -o start.bin
ls -al
file start.bin
nm start.bin
gcc -o main.bin main.c
file main.bin
nm main.bin


