#!/bin/bash
gcc -E main.c
gcc -o main.bin -g main.c
./main.bin
echo "====================================================="
gcc -o main.bin -g main.c -v
