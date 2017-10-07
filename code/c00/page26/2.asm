section .text
    mov eax,0x10
    jmp $
section file2data
    file2var db 3
section file2text
    global print
print:
    mov edx,[esp+8]
    mov ecx,[esp+4]
    mov ebx,1
    mov eax,4
    int 0x80
    ret
