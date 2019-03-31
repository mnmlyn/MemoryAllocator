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
#define COST_INUSE 0x20 /* 第6bit置位 */
#define COST_SIZE 0x1f  /* 低5bit置位 */

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
 * @cost    开销，记录块使用情况及块大小，低5bit记录大小，
 *          第6bit为1标识使用，为0标识free
 */
static struct {
    int nBuf;
    int szMin;
    int nBlock;
    int fList[MAXLOGSIZE + 1];
    u8 *buf;
    u8 *cost;
} memGlo;

/**
 * @function    LINK_INSERT
 * @brief       将第offset块开始的1<<logSize个分配单元，插入free链
 * @offset      待标记块起始分配单元
 * @logSize     待标记块的大小取log
 */
void LINK_INSERT(int offset, int logSize) {
    FreeLink *blk = (FreeLink *)(memGlo.buf + offset * memGlo.szMin);
    blk->prev = -1;
    blk->next = memGlo.fList[logSize];
    memGlo.fList[logSize] = offset;
    memGlo.cost[offset] = logSize;
}

/**
 * @function    LINK_DELETE
 * @brief       将连续的大小1<<logSize个分配单元，从free链中移除
 * @logSize     待标记块的大小取log
 * @return      找到的块的offset
 */
int LINK_DELETE(int logSize) {
    int offset = memGlo.fList[logSize];
    if (offset == -1)
        return -1;
    FreeLink *blk = (FreeLink *)(memGlo.buf + offset * memGlo.szMin);
    if (blk->next != -1) {
        FreeLink *blk1 = (FreeLink *)(memGlo.buf 
                + blk->next * memGlo.szMin);
        blk1->prev = -1;
    }
    memGlo.fList[logSize] = blk->next;
    memGlo.cost[offset] = COST_INUSE | logSize;
    return offset;
}

/**
 * @function    memalloc_init
 * @brief       内存管理程序初始化
 * @buf         被管理内存的起始地址
 * @size        被管理内存的字节数
 * @szMin       最小可分配内存的字节数，应>=sizeof(int)*2
 */
void memalloc_init(u8 *buf, int size, int szMin) {
    memGlo.buf = buf;
    memGlo.nBuf = size;
    if (szMin < sizeof(int) * 2)
        szMin = sizeof(int) * 2;
    memGlo.szMin = szMin;
    memGlo.nBlock = size / (szMin + sizeof(u8));/* 1 byte的开销 */
    memGlo.cost = buf + szMin * memGlo.nBlock;
    for (int i = 0; i <= MAXLOGSIZE; ++i) {
        memGlo.fList[i] = -1;
    }
    int offset = 0;
    for (int i = MAXLOGSIZE; i>=0; --i) {
        int blksize = 1<<i;
        if (offset + blksize > memGlo.nBlock)
            continue;
        LINK_INSERT(offset, i);
        offset += blksize;
    }
}

/**
 * @function    memalloc_malloc
 * @brief       分配一块指定大小的内存，返回获得的内存地址
 * @size        请求内存的大小
 * @return      获得的内存的地址
 */
void *memalloc_malloc(int size) {
    if (size <= 0)
        return nullptr;
    int i;
    for (i = 0; i <= MAXLOGSIZE; ++i) {
        if ((1 << i) * memGlo.szMin >= size)
            break;
    }
    int j;
    for (j = i; j <= MAXLOGSIZE; ++j) {
        if (memGlo.fList[j] != -1)
            break;
    }
    if (j > MAXLOGSIZE)
        return nullptr;
    /* 2^i为足够容纳size的内存大小，2^j为足够容纳2^i的空闲块大小 */
    int offset = LINK_DELETE(j);
    if (j != i) {
        while (j > i) {
            --j;
            LINK_INSERT(offset + (1<<j), j);
        }
    }
    memGlo.cost[offset] = COST_INUSE | i;
    return (void *)(memGlo.buf + offset * memGlo.szMin);
}












