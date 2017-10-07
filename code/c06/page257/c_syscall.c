/*
* =====================================================================================
*
*       Filename:  c_syscall.c
*
*    Description:  
*
*        Version:  1.0
*        Created:  2017年10月03日 10时53分20秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  YOUR NAME (), 
*   Organization:  
*
* =====================================================================================
*/
#include <unistd.h>
int 
main()
{
    write(0,"stdin:in\n",9);
    write(1,"stdout:hello\n",13);
    write(2,"stderror:fuck\n",14);
    return 0;
}

