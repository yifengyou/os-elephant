#!/bin/bash 
/usr/bin/nasm -f elf  -o syscall_write.o syscall_write.S 
/usr/bin/ld -m elf_i386 -o syscall_write.bin syscall_write.o
./syscall_write.bin
