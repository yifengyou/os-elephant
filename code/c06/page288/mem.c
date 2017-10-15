#include <stdio.h>

int main()
{
    int in_a=1,in_b=2;
    printf("in_a is %d,in_b is %d\n",in_a,in_b);
    asm("movb %%eax,%%ebx;"::"a"(in_a),"b"(in_b));
    printf("now in_a is %d,in_b is %d\n",in_a,in_b);
    return 0;
}
