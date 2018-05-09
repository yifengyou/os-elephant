#ifndef __USERPROG_WAITEXIT_H
#define __USERPROG_WAITEXIT_H
#include "thread.h"
pid_t sys_wait(int32_t* status);
void sys_exit(int32_t status);
#endif 
