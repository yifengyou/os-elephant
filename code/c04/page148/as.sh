#!/bin/bash 
nasm -f bin 16sreg_push.S  -o 16sreg_push.o 
objdump -D -b binary -m i8086 16sreg_push.o
