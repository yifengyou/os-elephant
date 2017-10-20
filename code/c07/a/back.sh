#!/bin/bash
if [ ! -d "./out" ]; then
    mkdir out
fi
if [ -e "hd3M.img" ]; then
    rm -rf hd3M.img  
fi
if [ ! -d "./out/boot" ];then 
    mkdir out/boot
fi 
if [ ! -d "./out/kernel" ];then 
    mkdir out/kernel 
fi 

echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成hd3M.img \033[0m"
/usr/bin/bximage -hd -mode="flat" -size=3 -q hd3M.img 
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成out/boot/mbr.bin \033[0m"
nasm -I ./boot/include/ -o ./out/boot/mbr.bin ./boot/mbr.S && dd if=./out/boot/mbr.bin of=./hd3M.img bs=512 count=1  conv=notrunc 
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成out/boot/loader.bin \033[0m"
nasm -I ./boot/include/ -o ./out/boot/loader.bin ./boot/loader.S && dd if=./out/boot/loader.bin of=./hd3M.img bs=512 count=4 seek=2 conv=notrunc 
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成out/kernel/print.o \033[0m"
nasm -f elf -o out/kernel/print.o lib/kernel/print.S
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成kernel.o \033[0m"
nasm -f elf -o out/kernel/kernel.o kernel/kernel.S
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成kernel.i \033[0m"
nasm -E -o kernel/kernel.i kernel/kernel.S
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成out/kernel/main.o \033[0m"
gcc -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin -o out/kernel/main.o kernel/main.c
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成out/interrupt.o \033[0m"
gcc -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin -o out/interrupt.o kernel/interrupt.c
echo -e "\033[32m==================================================================================================== \033[0m \033[0m"
echo -e "\033[32m生成out/init.o"
gcc -m32 -I lib/kernel/ -I lib/ -I kernel/ -c -fno-builtin -o out/init.o kernel/init.c
echo -e "\033[32m==================================================================================================== \033[0m"
echo -e "\033[32m生成out/kernel/kernel.bin mkimage \033[0m"
ld -melf_i386  -Ttext 0xc0001500 -e main -o ./out/kernel/kernel.bin out/kernel/main.o out/kernel/print.o out/init.o out/interrupt.o out/kernel/kernel.o && \
    dd if=./out/kernel/kernel.bin of=./hd3M.img bs=512 count=200 seek=9 conv=notrunc 
echo -e "\033[32m==================================================================================================== \033[0m"
tree out/

