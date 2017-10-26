clean:
	rm *.o *.bin -rf
test2.o:test2.c
	@gcc -c -o test2.o test2.c
test1.o:test1.c
	@gcc -c -o test1.o test1.c
objfiles = test1.o test2.o
test.bin:$(objfiles)
	@gcc -o test.bin test1.o test2.o
all:test.bin
	@echo "compile done"
