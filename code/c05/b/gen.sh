#!/bin/bash
if [ ! -d "./out" ]; then
      mkdir out
fi
if [ -e "hd3M.img" ]; then
     rm -rf hd3M.img  
fi
if [ ! -d "./out/boot" ]; then
      mkdir out/boot/
fi
/usr/bin/bximage -hd -mode="flat" -size=3 -q hd3M.img
nasm -I include/ -o ./out/boot/mbr.bin ./boot/mbr.S && dd if=./out/boot/mbr.bin of=./hd3M.img bs=512 count=1  conv=notrunc
nasm -I include/ -o ./out/boot/loader.bin ./boot/loader.S && dd if=./out/boot/loader.bin of=./hd3M.img bs=512 count=4 seek=2 conv=notrunc




