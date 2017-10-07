/*
* =====================================================================================
*
*       Filename:  base_asm.c
*
*    Description:  
*
*        Version:  1.0
*        Created:  2017年10月04日 10时16分30秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  YOUR NAME (), 
*   Organization:  
*
* =====================================================================================
*/
#include <stdio.h>
int in_a=666,in_b=333,out_sum;
int 
main()
{
    asm("\
        pusha;\
        movl in_a,%eax;\
        movl in_b,%ebx;\
        addl %ebx,%eax;\
        movl %eax,out_sum;\
        popa;\
        ");
    printf("sum is %d\n",out_sum);
    return 0 ;
}
