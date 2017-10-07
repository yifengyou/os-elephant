#!/bin/bash 
gcc c_syscall.c -o c_syscall.bin
echo './c_syscall.bin 2> stderror.txt'
./c_syscall.bin 2> stderror.txt
echo './c_syscall.bin 1> stdout.txt'
./c_syscall.bin 1> stdout.txt 
echo './c_syscall.bin 0> stdin.txt'
./c_syscall.bin 0> stdin.txt
echo './c_syscall.bin'
./c_syscall.bin 

