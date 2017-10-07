/*
* =====================================================================================
*
*       Filename:  reg_constraint.c
*
*    Description:  
*
*        Version:  1.0
*        Created:  2017年10月04日 10时20分48秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  YOUR NAME (), 
*   Organization:  
*
* =====================================================================================
*/
#include <stdio.h>
int 
main()
{
    int in_a=1,in_b=333,out_sum;
    asm("addl %%ebx,%%eax":"=a"(out_sum):"a"(in_a),"b"(in_b));
    printf("sum is %d\n",out_sum);
    return 0;
}
