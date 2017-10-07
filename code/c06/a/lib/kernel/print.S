TI_GDT equ  0
RPL0  equ   0
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

[bits 32]
section .text
;------------------------   put_char   -----------------------------
;功能描述:把栈中的1个字符写入光标所在处
;-------------------------------------------------------------------   
global put_char
put_char:
   pushad	   ;备份32位寄存器环境
   ;需要保证gs中为正确的视频段选择子,为保险起见,每次打印时都为gs赋值
   mov ax, SELECTOR_VIDEO	       ; 不能直接把立即数送入段寄存器
   mov gs, ax

;;;;;;;;;  获取当前光标位置 ;;;;;;;;;
   ;先获得高8位
   mov dx, 0x03d4  ;索引寄存器
   mov al, 0x0e	   ;用于提供光标位置的高8位
   out dx, al
   mov dx, 0x03d5  ;通过读写数据端口0x3d5来获得或设置光标位置 
   in al, dx	   ;得到了光标位置的高8位
   mov ah, al

   ;再获取低8位
   mov dx, 0x03d4
   mov al, 0x0f
   out dx, al
   mov dx, 0x03d5 
   in al, dx

   ;将光标存入bx
   mov bx, ax	  
   ;下面这行是在栈中获取待打印的字符
   mov ecx, [esp + 36]	      ;pushad压入4×8＝32字节,加上主调函数的返回地址4字节,故esp+36字节
   cmp cl, 0xd				  ;CR是0x0d,LF是0x0a
   jz .is_carriage_return
   cmp cl, 0xa
   jz .is_line_feed

   cmp cl, 0x8				  ;BS(backspace)的asc码是8
   jz .is_backspace
   jmp .put_other	   
;;;;;;;;;;;;;;;;;;

 .is_backspace:		      
;;;;;;;;;;;;       backspace的一点说明	     ;;;;;;;;;;
; 当为backspace时,本质上只要将光标移向前一个显存位置即可.后面再输入的字符自然会覆盖此处的字符
; 但有可能在键入backspace后并不再键入新的字符,这时在光标已经向前移动到待删除的字符位置,但字符还在原处,
; 这就显得好怪异,所以此处添加了空格或空字符0
   dec bx
   shl bx,1
   mov byte [gs:bx], 0x20		  ;将待删除的字节补为0或空格皆可
   inc bx
   mov byte [gs:bx], 0x07
   shr bx,1
   jmp .set_cursor
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 .put_other:
   shl bx, 1				  ; 光标位置是用2字节表示,将光标值乘2,表示对应显存中的偏移字节
   mov [gs:bx], cl			  ; ascii字符本身
   inc bx
   mov byte [gs:bx],0x07		  ; 字符属性
   shr bx, 1				  ; 恢复老的光标值
   inc bx				  ; 下一个光标值
   cmp bx, 2000		   
   jl .set_cursor			  ; 若光标值小于2000,表示未写到显存的最后,则去设置新的光标值
					  ; 若超出屏幕字符数大小(2000)则换行处理
 .is_line_feed:				  ; 是换行符LF(\n)
 .is_carriage_return:			  ; 是回车符CR(\r)
					  ; 如果是CR(\r),只要把光标移到行首就行了。
   xor dx, dx				  ; dx是被除数的高16位,清0.
   mov ax, bx				  ; ax是被除数的低16位.
   mov si, 80				  ; 由于是效仿linux，linux中\n便表示下一行的行首，所以本系统中，
   div si				  ; 把\n和\r都处理为linux中\n的意思，也就是下一行的行首。
   sub bx, dx				  ; 光标值减去除80的余数便是取整
					  ; 以上4行处理\r的代码

 .is_carriage_return_end:		  ; 回车符CR处理结束
   add bx, 80
   cmp bx, 2000
 .is_line_feed_end:			  ; 若是LF(\n),将光标移+80便可。  
   jl .set_cursor

;屏幕行范围是0~24,滚屏的原理是将屏幕的1~24行搬运到0~23行,再将第24行用空格填充
 .roll_screen:				  ; 若超出屏幕大小，开始滚屏
   cld  
   mov ecx, 960				  ; 一共有2000-80=1920个字符要搬运,共1920*2=3840字节.一次搬4字节,共3840/4=960次 
   mov esi, 0xb80a0			  ; 第1行行首
   mov edi, 0xb8000			  ; 第0行行首
   rep movsd				  

;;;;;;;将最后一行填充为空白
   mov ebx, 3840			  ; 最后一行首字符的第一个字节偏移= 1920 * 2
   mov ecx, 80				  ;一行是80字符(160字节),每次清空1字符(2字节),一行需要移动80次
 .cls:
   mov word [gs:ebx], 0x0720		  ;0x0720是黑底白字的空格键
   add ebx, 2
   loop .cls 
   mov bx,1920				  ;将光标值重置为1920,最后一行的首字符.

 .set_cursor:   
					  ;将光标设为bx值
;;;;;;; 1 先设置高8位 ;;;;;;;;
   mov dx, 0x03d4			  ;索引寄存器
   mov al, 0x0e				  ;用于提供光标位置的高8位
   out dx, al
   mov dx, 0x03d5			  ;通过读写数据端口0x3d5来获得或设置光标位置 
   mov al, bh
   out dx, al

;;;;;;; 2 再设置低8位 ;;;;;;;;;
   mov dx, 0x03d4
   mov al, 0x0f
   out dx, al
   mov dx, 0x03d5 
   mov al, bl
   out dx, al
 .put_char_done: 
   popad
   ret
