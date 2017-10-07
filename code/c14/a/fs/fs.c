#include "fs.h"
#include "super_block.h"
#include "inode.h"
#include "dir.h"
#include "stdint.h"
#include "stdio-kernel.h"
#include "list.h"
#include "string.h"
#include "ide.h"
#include "global.h"
#include "debug.h"
#include "memory.h"

/* 格式化分区,也就是初始化分区的元信息,创建文件系统 */
static void partition_format(struct partition* part) {
/* 为方便实现,一个块大小是一扇区 */
   uint32_t boot_sector_sects = 1;	  
   uint32_t super_block_sects = 1;
   uint32_t inode_bitmap_sects = DIV_ROUND_UP(MAX_FILES_PER_PART, BITS_PER_SECTOR);	   // I结点位图占用的扇区数.最多支持4096个文件
   uint32_t inode_table_sects = DIV_ROUND_UP(((sizeof(struct inode) * MAX_FILES_PER_PART)), SECTOR_SIZE);
   uint32_t used_sects = boot_sector_sects + super_block_sects + inode_bitmap_sects + inode_table_sects;
   uint32_t free_sects = part->sec_cnt - used_sects;  

/************** 简单处理块位图占据的扇区数 ***************/
   uint32_t block_bitmap_sects;
   block_bitmap_sects = DIV_ROUND_UP(free_sects, BITS_PER_SECTOR);
   /* block_bitmap_bit_len是位图中位的长度,也是可用块的数量 */
   uint32_t block_bitmap_bit_len = free_sects - block_bitmap_sects; 
   block_bitmap_sects = DIV_ROUND_UP(block_bitmap_bit_len, BITS_PER_SECTOR); 
/*********************************************************/
   
   /* 超级块初始化 */
   struct super_block sb;
   sb.magic = 0x19590318;
   sb.sec_cnt = part->sec_cnt;
   sb.inode_cnt = MAX_FILES_PER_PART;
   sb.part_lba_base = part->start_lba;

   sb.block_bitmap_lba = sb.part_lba_base + 2;	 // 第0块是引导块,第1块是超级块
   sb.block_bitmap_sects = block_bitmap_sects;

   sb.inode_bitmap_lba = sb.block_bitmap_lba + sb.block_bitmap_sects;
   sb.inode_bitmap_sects = inode_bitmap_sects;

   sb.inode_table_lba = sb.inode_bitmap_lba + sb.inode_bitmap_sects;
   sb.inode_table_sects = inode_table_sects; 

   sb.data_start_lba = sb.inode_table_lba + sb.inode_table_sects;
   sb.root_inode_no = 0;
   sb.dir_entry_size = sizeof(struct dir_entry);

   printk("%s info:\n", part->name);
   printk("   magic:0x%x\n   part_lba_base:0x%x\n   all_sectors:0x%x\n   inode_cnt:0x%x\n   block_bitmap_lba:0x%x\n   block_bitmap_sectors:0x%x\n   inode_bitmap_lba:0x%x\n   inode_bitmap_sectors:0x%x\n   inode_table_lba:0x%x\n   inode_table_sectors:0x%x\n   data_start_lba:0x%x\n", sb.magic, sb.part_lba_base, sb.sec_cnt, sb.inode_cnt, sb.block_bitmap_lba, sb.block_bitmap_sects, sb.inode_bitmap_lba, sb.inode_bitmap_sects, sb.inode_table_lba, sb.inode_table_sects, sb.data_start_lba);

   struct disk* hd = part->my_disk;
/*******************************
 * 1 将超级块写入本分区的1扇区 *
 ******************************/
   ide_write(hd, part->start_lba + 1, &sb, 1);
   printk("   super_block_lba:0x%x\n", part->start_lba + 1);

/* 找出数据量最大的元信息,用其尺寸做存储缓冲区*/
   uint32_t buf_size = (sb.block_bitmap_sects >= sb.inode_bitmap_sects ? sb.block_bitmap_sects : sb.inode_bitmap_sects);
   buf_size = (buf_size >= sb.inode_table_sects ? buf_size : sb.inode_table_sects) * SECTOR_SIZE;
   uint8_t* buf = (uint8_t*)sys_malloc(buf_size);	// 申请的内存由内存管理系统清0后返回
   
/**************************************
 * 2 将块位图初始化并写入sb.block_bitmap_lba *
 *************************************/
   /* 初始化块位图block_bitmap */
   buf[0] |= 0x01;       // 第0个块预留给根目录,位图中先占位
   uint32_t block_bitmap_last_byte = block_bitmap_bit_len / 8;
   uint8_t  block_bitmap_last_bit  = block_bitmap_bit_len % 8;
   uint32_t last_size = SECTOR_SIZE - (block_bitmap_last_byte % SECTOR_SIZE);	     // last_size是位图所在最后一个扇区中，不足一扇区的其余部分

   /* 1 先将位图最后一字节到其所在的扇区的结束全置为1,即超出实际块数的部分直接置为已占用*/
   memset(&buf[block_bitmap_last_byte], 0xff, last_size);
   
   /* 2 再将上一步中覆盖的最后一字节内的有效位重新置0 */
   uint8_t bit_idx = 0;
   while (bit_idx <= block_bitmap_last_bit) {
      buf[block_bitmap_last_byte] &= ~(1 << bit_idx++);
   }
   ide_write(hd, sb.block_bitmap_lba, buf, sb.block_bitmap_sects);

/***************************************
 * 3 将inode位图初始化并写入sb.inode_bitmap_lba *
 ***************************************/
   /* 先清空缓冲区*/
   memset(buf, 0, buf_size);
   buf[0] |= 0x1;      // 第0个inode分给了根目录
   /* 由于inode_table中共4096个inode,位图inode_bitmap正好占用1扇区,
    * 即inode_bitmap_sects等于1, 所以位图中的位全都代表inode_table中的inode,
    * 无须再像block_bitmap那样单独处理最后一扇区的剩余部分,
    * inode_bitmap所在的扇区中没有多余的无效位 */
   ide_write(hd, sb.inode_bitmap_lba, buf, sb.inode_bitmap_sects);

/***************************************
 * 4 将inode数组初始化并写入sb.inode_table_lba *
 ***************************************/
 /* 准备写inode_table中的第0项,即根目录所在的inode */
   memset(buf, 0, buf_size);  // 先清空缓冲区buf
   struct inode* i = (struct inode*)buf; 
   i->i_size = sb.dir_entry_size * 2;	 // .和..
   i->i_no = 0;   // 根目录占inode数组中第0个inode
   i->i_sectors[0] = sb.data_start_lba;	     // 由于上面的memset,i_sectors数组的其它元素都初始化为0 
   ide_write(hd, sb.inode_table_lba, buf, sb.inode_table_sects);
   
/***************************************
 * 5 将根目录初始化并写入sb.data_start_lba
 ***************************************/
   /* 写入根目录的两个目录项.和.. */
   memset(buf, 0, buf_size);
   struct dir_entry* p_de = (struct dir_entry*)buf;

   /* 初始化当前目录"." */
   memcpy(p_de->filename, ".", 1);
   p_de->i_no = 0;
   p_de->f_type = FT_DIRECTORY;
   p_de++;

   /* 初始化当前目录父目录".." */
   memcpy(p_de->filename, "..", 2);
   p_de->i_no = 0;   // 根目录的父目录依然是根目录自己
   p_de->f_type = FT_DIRECTORY;

   /* sb.data_start_lba已经分配给了根目录,里面是根目录的目录项 */
   ide_write(hd, sb.data_start_lba, buf, 1);

   printk("   root_dir_lba:0x%x\n", sb.data_start_lba);
   printk("%s format done\n", part->name);
   sys_free(buf);
}

/* 在磁盘上搜索文件系统,若没有则格式化分区创建文件系统 */
void filesys_init() {
   uint8_t channel_no = 0, dev_no, part_idx = 0;

   /* sb_buf用来存储从硬盘上读入的超级块 */
   struct super_block* sb_buf = (struct super_block*)sys_malloc(SECTOR_SIZE);

   if (sb_buf == NULL) {
      PANIC("alloc memory failed!");
   }
   printk("searching filesystem......\n");
   while (channel_no < channel_cnt) {
      dev_no = 0;
      while(dev_no < 2) {
	 if (dev_no == 0) {   // 跨过裸盘hd60M.img
	    dev_no++;
	    continue;
	 }
	 struct disk* hd = &channels[channel_no].devices[dev_no];
	 struct partition* part = hd->prim_parts;
	 while(part_idx < 12) {   // 4个主分区+8个逻辑
	    if (part_idx == 4) {  // 开始处理逻辑分区
	       part = hd->logic_parts;
	    }
	 
	 /* channels数组是全局变量,默认值为0,disk属于其嵌套结构,
	  * partition又为disk的嵌套结构,因此partition中的成员默认也为0.
	  * 若partition未初始化,则partition中的成员仍为0. 
	  * 下面处理存在的分区. */
	    if (part->sec_cnt != 0) {  // 如果分区存在
	       memset(sb_buf, 0, SECTOR_SIZE);

	       /* 读出分区的超级块,根据魔数是否正确来判断是否存在文件系统 */
	       ide_read(hd, part->start_lba + 1, sb_buf, 1);   

	       /* 只支持自己的文件系统.若磁盘上已经有文件系统就不再格式化了 */
	       if (sb_buf->magic == 0x19590318) {
		  printk("%s has filesystem\n", part->name);
	       } else {			  // 其它文件系统不支持,一律按无文件系统处理
		  printk("formatting %s`s partition %s......\n", hd->name, part->name);
		  partition_format(part);
	       }
	    }
	    part_idx++;
	    part++;	// 下一分区
	 }
	 dev_no++;	// 下一磁盘
      }
      channel_no++;	// 下一通道
   }
   sys_free(sb_buf);
}

