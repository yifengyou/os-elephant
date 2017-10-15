#include <stdio.h>
void main()
{
    int in_a = 18,in_b = 3;
    printf("in_a = %d,in_b = %d\n",in_a,in_b);
    asm("add %%ebx,%%eax":"+a"(in_a):"b"(in_b) );
    printf("now in_a = %d,in_b = %d\n",in_a,in_b);
}
