#!/bin/bash
if [ ! -d "./out" ]; then
      mkdir out
fi
if [ -e "hd3M.img" ]; then
     rm -rf hd3M.img  
fi
/usr/bin/bximage -hd -mode="flat" -size=3 -q hd3M.img
nasm -I include/ -o ./out/mbr.bin mbr.S && dd if=./out/mbr.bin of=./hd3M.img bs=512 count=1  conv=notrunc
nasm -I include/ -o ./out/loader.bin loader.S && dd if=./out/loader.bin of=./hd3M.img bs=512 count=1 seek=2 conv=notrunc
