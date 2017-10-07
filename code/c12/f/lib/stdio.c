#include "stdio.h"
#include "interrupt.h"
#include "global.h"
#include "string.h"
#include "syscall.h"
#include "print.h"

#define va_start(ap, v) ap = (va_list)&v  // 把ap指向第一个固定参数v
#define va_arg(ap, t) *((t*)(ap += 4))	  // ap指向下一个参数并返回其值
#define va_end(ap) ap = NULL		  // 清除ap

/* 将整型转换成字符(integer to ascii) */
static void itoa(uint32_t value, char** buf_ptr_addr, uint8_t base) {
   uint32_t m = value % base;	    // 求模,最先掉下来的是最低位   
   uint32_t i = value / base;	    // 取整
   if (i) {			    // 如果倍数不为0则递归调用。
      itoa(i, buf_ptr_addr, base);
   }
   if (m < 10) {      // 如果余数是0~9
      *((*buf_ptr_addr)++) = m + '0';	  // 将数字0~9转换为字符'0'~'9'
   } else {	      // 否则余数是A~F
      *((*buf_ptr_addr)++) = m - 10 + 'A'; // 将数字A~F转换为字符'A'~'F'
   }
}

/* 将参数ap按照格式format输出到字符串str,并返回替换后str长度 */
uint32_t vsprintf(char* str, const char* format, va_list ap) {
   char* buf_ptr = str;
   const char* index_ptr = format;
   char index_char = *index_ptr;
   int32_t arg_int;
   char* arg_str;
   while(index_char) {
      if (index_char != '%') {
	 *(buf_ptr++) = index_char;
	 index_char = *(++index_ptr);
	 continue;
      }
      index_char = *(++index_ptr);	 // 得到%后面的字符
      switch(index_char) {
	 case 's':
	    arg_str = va_arg(ap, char*);
	    strcpy(buf_ptr, arg_str);
	    buf_ptr += strlen(arg_str);
	    index_char = *(++index_ptr);
	    break;

	 case 'c':
	    *(buf_ptr++) = va_arg(ap, char);
	    index_char = *(++index_ptr);
	    break;

	 case 'd':
	    arg_int = va_arg(ap, int);
      /* 若是负数, 将其转为正数后,再正数前面输出个负号'-'. */
	    if (arg_int < 0) {
	       arg_int = 0 - arg_int;
	       *buf_ptr++ = '-';
	    }
	    itoa(arg_int, &buf_ptr, 10); 
	    index_char = *(++index_ptr);
	    break;

	 case 'x':
	    arg_int = va_arg(ap, int);
	    itoa(arg_int, &buf_ptr, 16); 
	    index_char = *(++index_ptr); // 跳过格式字符并更新index_char
	    break;
      }
   }
   return strlen(str);
}

/* 同printf不同的地方就是字符串不是写到终端,而是写到buf中 */
uint32_t sprintf(char* buf, const char* format, ...) {
   va_list args;
   uint32_t retval;
   va_start(args, format);
   retval = vsprintf(buf, format, args);
   va_end(args);
   return retval;
}

/* 格式化输出字符串format */
uint32_t printf(const char* format, ...) {
   va_list args;
   va_start(args, format);	       // 使args指向format
   char buf[1024] = {0};	       // 用于存储拼接后的字符串
   vsprintf(buf, format, args);
   va_end(args);
   return write(buf); 
}
