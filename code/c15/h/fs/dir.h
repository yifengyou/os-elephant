#ifndef __FS_DIR_H
#define __FS_DIR_H
#include "stdint.h"
#include "inode.h"
#include "fs.h"
#include "ide.h"
#include "global.h"

#define MAX_FILE_NAME_LEN  16	 // 最大文件名长度

/* 目录结构 */
struct dir {
   struct inode* inode;   
   uint32_t dir_pos;	  // 记录在目录内的偏移
   uint8_t dir_buf[512];  // 目录的数据缓存
};

/* 目录项结构 */
struct dir_entry {
   char filename[MAX_FILE_NAME_LEN];  // 普通文件或目录名称
   uint32_t i_no;		      // 普通文件或目录对应的inode编号
   enum file_types f_type;	      // 文件类型
};

extern struct dir root_dir;             // 根目录
void open_root_dir(struct partition* part);
struct dir* dir_open(struct partition* part, uint32_t inode_no);
void dir_close(struct dir* dir);
bool search_dir_entry(struct partition* part, struct dir* pdir, const char* name, struct dir_entry* dir_e);
void create_dir_entry(char* filename, uint32_t inode_no, uint8_t file_type, struct dir_entry* p_de);
bool sync_dir_entry(struct dir* parent_dir, struct dir_entry* p_de, void* io_buf);
bool delete_dir_entry(struct partition* part, struct dir* pdir, uint32_t inode_no, void* io_buf);
struct dir_entry* dir_read(struct dir* dir);
bool dir_is_empty(struct dir* dir);
int32_t dir_remove(struct dir* parent_dir, struct dir* child_dir);
#endif
