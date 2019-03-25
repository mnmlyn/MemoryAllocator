/**
 * 内存管理程序
 * 使用buddy伙伴算法，参考sqlite3中mem5的实现
 * 作者：mnmlyn@163.com
 * 日期：2019年3月25日
 * 参考：https://www.sqlite.org
 */

/**
 * 要求：
 * 给定一块内存，实现该内存块上的malloc、free、realloc等函数，
 * 不能再动态分配内存，可以使用O(1)的栈和全局空间。
 * 算法思路：
 * 1. 设定最小分配单元，一次只能分配2的幂次个最小分配单元的内存。
 * 2. 用一个数组（链表的数组），记录不同幂次的空闲内存块。
 * 3. 每一个最小分配单元，都对应有一个字节的开销，记录其是否空闲
 * 以及空闲块的大小。连续可合并的空闲块，只记录在第一个块开销中。
 * 4. 初始化时，从尽可能大的空闲块开始
 * 5. 分配时，找到最小可满足需求的块，切分为刚好满足需求的2幂次
 * 大小，将切分后多余的块存入空闲数组的对应链中。
 * 6. 释放时，若其伙伴块为free状态，合并，直到伙伴不为free
 */
#define MAXLOGSIZE 30   /* log(可分配最大块包含的最小单元数) */

/**
 * @struct  FreeLink
 * @brief   空闲内存链的节点
 * @next    下一个同等大小空闲块的序号，-1为不存在
 * @prev    上一个同等大小空闲块的序号，-1为不存在
 */
typedef struct FreeLink {
    int next;
    int prev;
} FreeLink;
typedef unsigned char u8;

/**
 * @struct  memFlo
 * @brief   全局变量
 * @nBuf    待分配内存大小
 * @szMin   最小分配单元大小
 * @nBlock  可用最小分配单元的数量
 * @fList   空闲块链表数组
 * @buf     待分配内存
 * @cost    开销，记录块使用情况及块大小
 */
static struct {
    int nBuf;
    int szMin;
    int nBlock;
    int fList[MAXLOGSIZE + 1];
    u8 *buf;
    u8 *cost;
} memGlo;




