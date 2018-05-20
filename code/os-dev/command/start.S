[bits 32]
extern	 main
extern	 exit 
section .text
global _start
_start:
   ;下面这两个要和execv中load之后指定的寄存器一致
   push	 ebx	  ;压入argv
   push  ecx	  ;压入argc
   call  main

   ;将main的返回值通过栈传给exit,gcc用eax存储返回值,这是ABI规定的
   push  eax
   call exit
   ;exit不会返回

