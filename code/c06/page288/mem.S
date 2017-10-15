	.file	"mem.c"
	.section	.rodata
.LC0:
	.string	"in_a is %d,in_b is %d\n"
.LC1:
	.string	"now in_a is %d,in_b is %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x78,0x6
	.cfi_escape 0x10,0x3,0x2,0x75,0x7c
	subl	$16, %esp
	movl	$1, -12(%ebp)
	movl	$2, -16(%ebp)
	subl	$4, %esp
	pushl	-16(%ebp)
	pushl	-12(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	-16(%ebp), %edx
	movl	%edx, %ebx
#APP
# 7 "mem.c" 1
	movb %eax,%ebx;
# 0 "" 2
#NO_APP
	subl	$4, %esp
	pushl	-16(%ebp)
	pushl	-12(%ebp)
	pushl	$.LC1
	call	printf
	addl	$16, %esp
	movl	$0, %eax
	leal	-8(%ebp), %esp
	popl	%ecx
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 7.1.0"
	.section	.note.GNU-stack,"",@progbits
