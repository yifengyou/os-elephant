#!/bin/bash 
gcc -o main.bin main.c
file main.bin
nm main.bin




readelf -a main.bin 
