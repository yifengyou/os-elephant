#include <stdio.h>
void main()
{
    int in_a = 18,in_b = 3,out = 0;
    asm("divb %[divisor];movb %%al,%[result];"\
        :[result]"=m"(out) \
        :"a"(in_a),[divisor]"m"(in_b) \
       );
    printf("in_a = %d,in_b = %d,result = %d\n",in_a,in_b,out);
}
