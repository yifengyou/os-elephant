#include "print.h"
#include "init.h"
#include "thread.h"
#include "interrupt.h"
#include "console.h"
#include "process.h"
#include "syscall-init.h"
#include "syscall.h"
#include "stdio.h"
#include "memory.h"

void k_thread_a(void*);
void k_thread_b(void*);
void u_prog_a(void);
void u_prog_b(void);

int main(void) {
   put_str("I am kernel\n");
   init_all();
   intr_enable();
   thread_start("k_thread_a", 31, k_thread_a, "I am thread_a");
   thread_start("k_thread_b", 31, k_thread_b, "I am thread_b ");
   while(1);
   return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg) {     
   char* para = arg;
   void* addr1;
   void* addr2;
   void* addr3;
   void* addr4;
   void* addr5;
   void* addr6;
   void* addr7;
   console_put_str(" thread_a start\n");
   int max = 1000;
   while (max-- > 0) {
      int size = 128;
      addr1 = sys_malloc(size); 
      size *= 2; 
      addr2 = sys_malloc(size); 
      size *= 2; 
      addr3 = sys_malloc(size);
      sys_free(addr1);
      addr4 = sys_malloc(size);
      size *= 2; size *= 2; size *= 2; size *= 2; 
      size *= 2; size *= 2; size *= 2; 
      addr5 = sys_malloc(size);
      addr6 = sys_malloc(size);
      sys_free(addr5);
      size *= 2; 
      addr7 = sys_malloc(size);
      sys_free(addr6);
      sys_free(addr7);
      sys_free(addr2);
      sys_free(addr3);
      sys_free(addr4);
   }
   console_put_str(" thread_a end\n");
   while(1);
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) {     
   char* para = arg;
   void* addr1;
   void* addr2;
   void* addr3;
   void* addr4;
   void* addr5;
   void* addr6;
   void* addr7;
   void* addr8;
   void* addr9;
   int max = 1000;
   console_put_str(" thread_b start\n");
   while (max-- > 0) {
      int size = 9;
      addr1 = sys_malloc(size);
      size *= 2; 
      addr2 = sys_malloc(size);
      size *= 2; 
      sys_free(addr2);
      addr3 = sys_malloc(size);
      sys_free(addr1);
      addr4 = sys_malloc(size);
      addr5 = sys_malloc(size);
      addr6 = sys_malloc(size);
      sys_free(addr5);
      size *= 2; 
      addr7 = sys_malloc(size);
      sys_free(addr6);
      sys_free(addr7);
      sys_free(addr3);
      sys_free(addr4);

      size *= 2; size *= 2; size *= 2; 
      addr1 = sys_malloc(size);
      addr2 = sys_malloc(size);
      addr3 = sys_malloc(size);
      addr4 = sys_malloc(size);
      addr5 = sys_malloc(size);
      addr6 = sys_malloc(size);
      addr7 = sys_malloc(size);
      addr8 = sys_malloc(size);
      addr9 = sys_malloc(size);
      sys_free(addr1);
      sys_free(addr2);
      sys_free(addr3);
      sys_free(addr4);
      sys_free(addr5);
      sys_free(addr6);
      sys_free(addr7);
      sys_free(addr8);
      sys_free(addr9);
   }
   console_put_str(" thread_b end\n");
   while(1);
}

/* 测试用户进程 */
void u_prog_a(void) {
   char* name = "prog_a";
   printf(" I am %s, my pid:%d%c", name, getpid(),'\n');
   while(1);
}

/* 测试用户进程 */
void u_prog_b(void) {
   char* name = "prog_b";
   printf(" I am %s, my pid:%d%c", name, getpid(), '\n');
   while(1);
}
