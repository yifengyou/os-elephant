#ifndef __USERPROG_FORK_H
#define __USERPROG_FORK_H
#include "thread.h"
/* fork子进程,只能由用户进程通过系统调用fork调用,
   内核线程不可直接调用,原因是要从0级栈中获得esp3等 */
pid_t sys_fork(void);
#endif
