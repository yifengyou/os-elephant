#!/bin/bash
if [ ! -d "./out" ]; then
      mkdir out
fi
if [ -e "hd3M.img" ]; then
     rm -rf hd3M.img  
fi
/usr/bin/bximage -hd -mode="flat" -size=3 -q hd3M.img
nasm -I ./boot/include/ -o ./out/mbr.bin ./boot/mbr.S && dd if=./out/mbr.bin of=./hd3M.img bs=512 count=1  conv=notrunc
nasm -I ./boot/include/ -o ./out/loader.bin ./boot/loader.S && dd if=./out/loader.bin of=./hd3M.img bs=512 count=4 seek=2 conv=notrunc
if [ ! -d "./out/kernel" ];then 
    mkdir out/kernel 
fi 
gcc -c -o out/kernel/main.o kernel/main.c
/usr/bin/ld out/kernel/main.o -Ttext 0xc0001500 -e main -o out/kernel/kernel.bin && \
    dd if=./out/kernel/kernel.bin of=./hd3M.img bs=512 count=200 seek=9 conv=notrunc


