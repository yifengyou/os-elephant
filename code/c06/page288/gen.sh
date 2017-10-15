#!/bin/bash 
gcc -m32 mem.c -o mem.bin -g 
gcc -m32  -S -o mem.S mem.c
