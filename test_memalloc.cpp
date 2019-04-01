#include <iostream>
#include <stdlib.h>
#include "memalloc.h"
#include <gtest/gtest.h>

using namespace std;
typedef unsigned char u8;

#define BUFSIZE 1024

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}

TEST(MEMALLOC_TEST, TEST){
    int bufsize = 1024;
    int unitsize = 16;
    /* memGlo.nBlock == 60 */
    u8 *buf = (u8 *)malloc(bufsize); 
    memalloc_init((u8 *)buf, bufsize, unitsize);
    u8 *p1,*p2,*p3,*p4,*p5,*p6,*p7,*p8,*p9;
    
    /* 可分配最大内存块大小为32个最小分配单元 */
    EXPECT_EQ(memalloc_malloc(unitsize*60), (void *)0);
    EXPECT_EQ(p1=(u8 *)memalloc_malloc(unitsize*32), buf);
    EXPECT_EQ(memalloc_malloc(unitsize*32), (void *)0);
    EXPECT_EQ(p2=(u8 *)memalloc_malloc(unitsize*16), buf+unitsize*32);
    EXPECT_EQ(p3=(u8 *)memalloc_malloc(unitsize*8), buf+unitsize*48);
    EXPECT_EQ(p4=(u8 *)memalloc_malloc(unitsize*4), buf+unitsize*56);
    
    /* 分配完，不能继续分配 */
    EXPECT_EQ(memalloc_malloc(1), (void *)0);
    EXPECT_EQ(memalloc_malloc(unitsize*4), (void *)0);
    
    memalloc_free(p1);
    memalloc_free(p2);
    memalloc_free(p3);
    memalloc_free(p4);

    for (int i = 0; i < 5; ++i){

    /* 分配小内存，应在最后一个块中分配 */
    p1 = (u8 *)memalloc_malloc(1);
    p2 = (u8 *)memalloc_malloc(1);
    p3 = (u8 *)memalloc_malloc(1);
    p4 = (u8 *)memalloc_malloc(1);

    EXPECT_EQ((p1-buf)/unitsize, 56);
    EXPECT_EQ((p2-buf)/unitsize, 57);
    EXPECT_EQ((p3-buf)/unitsize, 58);
    EXPECT_EQ((p4-buf)/unitsize, 59);

    /* 分配9个最小分配单元，应分配第32块 */
    p5 = (u8 *)memalloc_malloc(unitsize*9);
    EXPECT_EQ((p5-buf)/unitsize, 32);

    /* 分配3个最小分配单元，由于最后4个占用，应拆分分配 */
    p6 = (u8 *)memalloc_malloc(unitsize*3);
    EXPECT_EQ((p6-buf)/unitsize, 48);
    
    p7 = (u8 *)memalloc_malloc(unitsize*5);
    EXPECT_EQ((p7-buf)/unitsize, 0);

    p8 = (u8 *)memalloc_malloc(unitsize*3);
    EXPECT_EQ((p8-buf)/unitsize, 52);

    p9 = (u8 *)memalloc_malloc(unitsize*15);
    EXPECT_EQ((p9-buf)/unitsize, 16);

    memalloc_free(p1);
    memalloc_free(p2);
    
    p1 = (u8 *)memalloc_malloc(unitsize*1);
    EXPECT_EQ((p1-buf)/unitsize, 56);

    memalloc_free(p1);
    memalloc_free(p3);
    memalloc_free(p4);

    p1 = (u8 *)memalloc_malloc(unitsize*5);
    EXPECT_EQ((p1-buf)/unitsize, 8);

    p2 = (u8 *)memalloc_malloc(unitsize*3);
    EXPECT_EQ((p2-buf)/unitsize, 56);

    EXPECT_EQ(memalloc_malloc(1), (void *)0);

    memalloc_free(p1);
    memalloc_free(p2);
    memalloc_free(p5);
    memalloc_free(p6);
    memalloc_free(p7);
    memalloc_free(p8);
    memalloc_free(p9);
    
    } /* end for (int i = 0; i < 5; ++i) */
    free(buf);
}
