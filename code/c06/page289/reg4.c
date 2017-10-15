#include <stdio.h>
void main()
{
    int in_a = 0x12345678, in_b = 0;
    asm("movw %1,%0;":"=m"(in_b):"a"(in_a));
    printf("word in_b is 0x%x\n", in_b);
    in_b = 0;

    asm("movb %1,%0;":"=m"(in_b):"a"(in_a));
    printf("low byte in_b is 0x%x\n", in_b);

    in_b = 0;
    asm("movb %h1,%0;":"=m"(in_b):"a"(in_a));
    printf("high byte in_b is 0x%x\n", in_b);
}
