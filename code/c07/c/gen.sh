#!/bin/bash
if [ ! -d "./build" ]; then
      mkdir build
fi
if [ -e "hd3M.img" ]; then
     rm -rf hd3M.img  
fi
if [ ! -d "./build/boot" ];then 
    mkdir build/boot
fi 
if [ ! -d "./build/kernel" ];then 
    mkdir build/kernel 
fi 

/usr/bin/bximage -hd -mode="flat" -size=3 -q hd3M.img

nasm -I ./boot/include/ -o ./build/boot/mbr.bin ./boot/mbr.S && dd if=./build/boot/mbr.bin of=./hd3M.img bs=512 count=1  conv=notrunc

nasm -I ./boot/include/ -o ./build/boot/loader.bin ./boot/loader.S && dd if=./build/boot/loader.bin of=./hd3M.img bs=512 count=4 seek=2 conv=notrunc

nasm -f elf -o build/kernel/print.o lib/kernel/print.S

nasm -f elf -o build/kernel/kernel.o kernel/kernel.S

gcc -m32 -I lib/kernel -c -o build/timer.o device/timer.c 

gcc -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin -o build/kernel/main.o kernel/main.c

gcc -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin -o build/interrupt.o kernel/interrupt.c

gcc -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin -o build/init.o kernel/init.c

ld -melf_i386  -Ttext 0xc0001500 -e main -o ./build/kernel/kernel.bin \
    build/kernel/main.o build/kernel/print.o build/init.o build/interrupt.o build/kernel/kernel.o build/timer.o  \
    && dd if=./build/kernel/kernel.bin of=./hd3M.img bs=512 count=200 seek=9 conv=notrunc




