#/bin/bash --version
nasm -f elf 1.asm -o 1.o
nasm -f elf 2.asm -o 2.o
ld -melf_i386 1.o 2.o -o 12
./12
echo "==========================================="
readelf -e ./12
