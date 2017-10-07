gcc -c -o kernel/main.o  kernel/main.c &&  ld kernel/main.o -Ttext 0xc0001500 -e main -o kernel/kernel.bin && dd if=kernel/kernel.bin of=/home/work/my_workspace/bochs/hd60M.img bs=512 count=200 seek=9 conv=notrunc 

