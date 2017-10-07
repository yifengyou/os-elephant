#include "process.h"
#include "global.h"
#include "debug.h"
#include "memory.h"
#include "thread.h"    
#include "list.h"    
#include "tss.h"    
#include "interrupt.h"
#include "string.h"
#include "console.h"

extern void intr_exit(void);

/* 构建用户进程初始上下文信息 */
void start_process(void* filename_) {
   void* function = filename_;
   struct task_struct* cur = running_thread();
   cur->self_kstack += sizeof(struct thread_stack);
   struct intr_stack* proc_stack = (struct intr_stack*)cur->self_kstack;	 
   proc_stack->edi = proc_stack->esi = proc_stack->ebp = proc_stack->esp_dummy = 0;
   proc_stack->ebx = proc_stack->edx = proc_stack->ecx = proc_stack->eax = 0;
   proc_stack->gs = 0;		 // 用户态用不上,直接初始为0
   proc_stack->ds = proc_stack->es = proc_stack->fs = SELECTOR_U_DATA;
   proc_stack->eip = function;	 // 待执行的用户程序地址
   proc_stack->cs = SELECTOR_U_CODE;
   proc_stack->eflags = (EFLAGS_IOPL_0 | EFLAGS_MBS | EFLAGS_IF_1);
   proc_stack->esp = (void*)((uint32_t)get_a_page(PF_USER, USER_STACK3_VADDR) + PG_SIZE) ;
   proc_stack->ss = SELECTOR_U_DATA; 
   asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (proc_stack) : "memory");
}

/* 击活页表 */
void page_dir_activate(struct task_struct* p_thread) {
/********************************************************
 * 执行此函数时,当前任务可能是线程。
 * 之所以对线程也要重新安装页表, 原因是上一次被调度的可能是进程,
 * 否则不恢复页表的话,线程就会使用进程的页表了。
 ********************************************************/

/* 若为内核线程,需要重新填充页表为0x100000 */
   uint32_t pagedir_phy_addr = 0x100000;  // 默认为内核的页目录物理地址,也就是内核线程所用的页目录表
   if (p_thread->pgdir != NULL)	{    // 用户态进程有自己的页目录表
      pagedir_phy_addr = addr_v2p((uint32_t)p_thread->pgdir);
   }

   /* 更新页目录寄存器cr3,使新页表生效 */
   asm volatile ("movl %0, %%cr3" : : "r" (pagedir_phy_addr) : "memory");
}

/* 击活线程或进程的页表,更新tss中的esp0为进程的特权级0的栈 */
void process_activate(struct task_struct* p_thread) {
   ASSERT(p_thread != NULL);
   /* 击活该进程或线程的页表 */
   page_dir_activate(p_thread);

   /* 内核线程特权级本身就是0,处理器进入中断时并不会从tss中获取0特权级栈地址,故不需要更新esp0 */
   if (p_thread->pgdir) {
      /* 更新该进程的esp0,用于此进程被中断时保留上下文 */
      update_tss_esp(p_thread);
   }
}

/* 创建页目录表,将当前页表的表示内核空间的pde复制,
 * 成功则返回页目录的虚拟地址,否则返回-1 */
uint32_t* create_page_dir(void) {

   /* 用户进程的页表不能让用户直接访问到,所以在内核空间来申请 */
   uint32_t* page_dir_vaddr = get_kernel_pages(1);
   if (page_dir_vaddr == NULL) {
      console_put_str("create_page_dir: get_kernel_page failed!");
      return NULL;
   }

/************************** 1  先复制页表  *************************************/
   /*  page_dir_vaddr + 0x300*4 是内核页目录的第768项 */
   memcpy((uint32_t*)((uint32_t)page_dir_vaddr + 0x300*4), (uint32_t*)(0xfffff000+0x300*4), 1024);
/*****************************************************************************/

/************************** 2  更新页目录地址 **********************************/
   uint32_t new_page_dir_phy_addr = addr_v2p((uint32_t)page_dir_vaddr);
   /* 页目录地址是存入在页目录的最后一项,更新页目录地址为新页目录的物理地址 */
   page_dir_vaddr[1023] = new_page_dir_phy_addr | PG_US_U | PG_RW_W | PG_P_1;
/*****************************************************************************/
   return page_dir_vaddr;
}

/* 创建用户进程虚拟地址位图 */
void create_user_vaddr_bitmap(struct task_struct* user_prog) {
   user_prog->userprog_vaddr.vaddr_start = USER_VADDR_START;
   uint32_t bitmap_pg_cnt = DIV_ROUND_UP((0xc0000000 - USER_VADDR_START) / PG_SIZE / 8 , PG_SIZE);
   user_prog->userprog_vaddr.vaddr_bitmap.bits = get_kernel_pages(bitmap_pg_cnt);
   user_prog->userprog_vaddr.vaddr_bitmap.btmp_bytes_len = (0xc0000000 - USER_VADDR_START) / PG_SIZE / 8;
   bitmap_init(&user_prog->userprog_vaddr.vaddr_bitmap);
}

/* 创建用户进程 */
void process_execute(void* filename, char* name) { 
   /* pcb内核的数据结构,由内核来维护进程信息,因此要在内核内存池中申请 */
   struct task_struct* thread = get_kernel_pages(1);
   init_thread(thread, name, default_prio); 
   create_user_vaddr_bitmap(thread);
   thread_create(thread, start_process, filename);
   thread->pgdir = create_page_dir();
   
   enum intr_status old_status = intr_disable();
   ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
   list_append(&thread_ready_list, &thread->general_tag);

   ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
   list_append(&thread_all_list, &thread->all_list_tag);
   intr_set_status(old_status);
}

