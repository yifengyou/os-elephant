char* str = "hello inlineasm\n";
int count = 0;
int main() {
	asm("pusha;\
		movl $4,%eax; \
		movl $1, %ebx; \
		movl str,%ecx; \
		movl $16,%edx; \
		int $0x80; \
		mov %eax,count; \
		popa; \
		");
}