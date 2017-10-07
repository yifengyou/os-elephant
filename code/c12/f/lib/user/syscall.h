#ifndef __LIB_USER_SYSCALL_H
#define __LIB_USER_SYSCALL_H
#include "stdint.h"
enum SYSCALL_NR {
   SYS_GETPID,
   SYS_WRITE
};
uint32_t getpid(void);
uint32_t write(char* str);
#endif

