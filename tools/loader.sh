nasm -I include/ -o loader.bin loader.S && dd if=./loader.bin  of=/home/work/my_workspace/bochs/hd60M.img bs=512 count=3 seek=2 conv=notrunc
