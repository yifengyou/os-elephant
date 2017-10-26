clean:
	rm *.o *.bin -rf
%.o:%.c
	gcc -c -o $@ $^
objfiles = test1.o test2.o
test.bin:$(objfiles)
	@gcc -o test.bin test1.o test2.o
all:test.bin
	@echo "compile done"
