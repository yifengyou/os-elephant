#!/bin/bash 
nasm -f bin 32sreg_push.S  -o 32sreg_push.o 
objdump -D -b binary -m i386 32sreg_push.o
