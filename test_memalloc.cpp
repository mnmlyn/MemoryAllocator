#include <iostream>
#include "memalloc.h"
#include <stdlib.h>

using namespace std;
typedef unsigned char u8;

#define BUFSIZE 1024

int main() {
    u8 *buf = (u8 *)malloc(BUFSIZE);
    memalloc_init(buf, BUFSIZE, 16);
    u8 * mbuf = (u8 *)memalloc_malloc(12);
    memalloc_free(mbuf);
    free(buf);
    cout << "End\n";
    return 0;
}
