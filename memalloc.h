/**
 * 内存管理程序头文件
 * 使用buddy伙伴算法，参考sqlite3中mem5的实现
 * 作者：mnmlyn@163.com
 * 日期：2019年3月25日
 * 参考：https://www.sqlite.org
 */
#ifndef _MEMALLOC_H_
#define _MEMALLOC_H_

/**
 * @function    memalloc_init
 * @brief       内存管理程序初始化
 * @buf         被管理内存的起始地址
 * @size        被管理内存的字节数
 * @szMin       最小可分配内存的字节数，应>=sizeof(int)*2
 */
void memalloc_init(unsigned char *buf, int size, int szMin);

/**
 * @function    memalloc_malloc
 * @brief       分配一块指定大小的内存，返回获得的内存地址
 * @size        请求内存的大小
 * @return      获得的内存的地址
 */
void *memalloc_malloc(int size);

/**
 * @function    memalloc_free
 * @brief       将分配的内存释放
 * @ptr         已分配内存的指针
 */
void memalloc_free(void *ptr);

#endif
