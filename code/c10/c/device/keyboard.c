#include "keyboard.h"
#include "print.h"
#include "interrupt.h"
#include "io.h"
#include "global.h"

#define KBD_BUF_PORT 0x60	   // 键盘buffer寄存器端口号为0x60

/* 键盘中断处理程序 */
static void intr_keyboard_handler(void) {
   put_char('k');
/* 必须要读取输出缓冲区寄存器,否则8042不再继续响应键盘中断 */
   inb(KBD_BUF_PORT);
   return;
}

/* 键盘初始化 */
void keyboard_init() {
   put_str("keyboard init start\n");
   register_handler(0x21, intr_keyboard_handler);
   put_str("keyboard init done\n");
}

