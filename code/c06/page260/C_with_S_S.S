section .data
str: db "asm_print say hi youyifeng!",0xa,0x00 
;0x0a是换行符，0x00是字符串结束符，不加的话会把后面字符陆续输出，直到遇到空白字符
;while循环遍历字符串统计字符串长度，遇空白字符结束。汇编不支持NULL
str_len equ $-str

section .text
extern c_print ;外部申明引用c_print
global _start
_start:
	push str
	call c_print
	add esp,4;自右向左调用参数，调用者清理栈空间
	mov eax,1;调用1号中断，#define __NR_exit 1
	int 0x80

global asm_print ;定义函数asm_print
asm_print:
	push ebp ;备份ebp 
	mov ebp,esp
    mov eax,4 ;定义调用4号中断，需要传入三个参数，#define __NR_write 4
	mov ebx,1 ;此参数为文件描述符，固定3个，0表示标准输入，1表示标准输出，2表示标准错误输出
	mov ecx,[ebp+8] ;长度参数len 
	mov edx,[ebp+12] ;字符串首地址参数  char *str
	int 0x80 ;功能号填写完毕，发起中断
    pop ebp ;回复ebp
	ret
