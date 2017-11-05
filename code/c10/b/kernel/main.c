#include "print.h"
#include "init.h"
#include "thread.h"
#include "interrupt.h"
#include "console.h"

void k_thread_a(void*);
void k_thread_b(void*);
void k_thread_c(void*);
void k_thread_d(void*);

int main(void) {
   put_str("I am kernel\n");
   init_all();

   thread_start("k_thread_a", 64, k_thread_a, "argA ");
   thread_start("k_thread_b", 32, k_thread_b, "argB ");
   thread_start("k_thread_c", 16, k_thread_c, "argC ");
   thread_start("k_thread_d", 8, k_thread_d, "argD ");

   intr_enable();
   while(1) {
      console_put_str("Main ");
   };
   return 0;
}

/* 在线程中运行的函数 */
void k_thread_a(void* arg) {     
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
   char* para = arg;
   while(1) {
      console_put_str(para);
   }
}

/* 在线程中运行的函数 */
void k_thread_b(void* arg) {     
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
   char* para = arg;
   while(1) {
      console_put_str(para);
   }
}

/* 在线程中运行的函数 */
void k_thread_c(void* arg) {     
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
   char* para = arg;
   while(1) {
      console_put_str(para);
   }
}

/* 在线程中运行的函数 */
void k_thread_d(void* arg) {     
/* 用void*来通用表示参数,被调用的函数知道自己需要什么类型的参数,自己转换再用 */
   char* para = arg;
   while(1) {
      console_put_str(para);
   }
}


