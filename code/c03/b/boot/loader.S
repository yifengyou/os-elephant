%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR

; 输出背景色绿色，前景色红色，并且跳动的字符串"1 MBR"
mov byte [gs:0x20],'2'
mov byte [gs:0x21],0xA4     ; A表示绿色背景闪烁，4表示前景色为红色

mov byte [gs:0x22],' '
mov byte [gs:0x23],0xA4

mov byte [gs:0x24],'L'
mov byte [gs:0x25],0xA4   

mov byte [gs:0x26],'O'
mov byte [gs:0x27],0xA4

mov byte [gs:0x28],'A'
mov byte [gs:0x29],0xA4

mov byte [gs:0x2a],'D'
mov byte [gs:0x2b],0xA4

mov byte [gs:0x2c],'E'
mov byte [gs:0x2d],0xA4

mov byte [gs:0x2e],'R'
mov byte [gs:0x2f],0xA4

jmp $		       ; 通过死循环使程序悬停在此

