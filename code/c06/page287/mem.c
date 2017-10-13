#include <stdio.h>

int main()
{
    int in_a=1,in_b=2;
    printf("in_b is %d\n",in_b);
    asm("movb %b0,%1;"::"a"(in_a),"m"(in_b));
    printf("in_b now is %d\n",in_b);
    return 0;
}
