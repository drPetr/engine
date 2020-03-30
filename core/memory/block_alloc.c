#include <core/alloc.h>
#include <core/common.h>

#include <stdlib.h>


#define M_UINT64        0xffffffffffffffff

#define ML_UINT64       0xffffffff00000000
#define MR_UINT64       0x00000000ffffffff

#define MLL_UINT64      0xffff000000000000
#define MLR_UINT64      0x0000ffff00000000
#define MRL_UINT64      0x00000000ffff0000
#define MRR_UINT64      0x000000000000ffff

#define MLLL_UINT64     0xff00000000000000
#define MLLR_UINT64     0x00ff000000000000
#define MLRL_UINT64     0x0000ff0000000000
#define MLRR_UINT64     0x000000ff00000000
#define MRLL_UINT64     0x00000000ff000000
#define MRLR_UINT64     0x0000000000ff0000
#define MRRL_UINT64     0x000000000000ff00
#define MRRR_UINT64     0x00000000000000ff

#define MLLLL_UINT64    0xf000000000000000
#define MLLLR_UINT64    0x0f00000000000000
#define MLLRL_UINT64    0x00f0000000000000
#define MLLRR_UINT64    0x000f000000000000
#define MLRLL_UINT64    0x0000f00000000000
#define MLRLR_UINT64    0x00000f0000000000
#define MLRRL_UINT64    0x000000f000000000
#define MLRRR_UINT64    0x0000000f00000000
#define MRLLL_UINT64    0x00000000f0000000
#define MRLLR_UINT64    0x000000000f000000
#define MRLRL_UINT64    0x0000000000f00000
#define MRLRR_UINT64    0x00000000000f0000
#define MRRLL_UINT64    0x000000000000f000
#define MRRLR_UINT64    0x0000000000000f00
#define MRRRL_UINT64    0x00000000000000f0
#define MRRRR_UINT64    0x000000000000000f


typedef struct {
    uint8_t*        memPtr;
    blocksInfo64_t  blocksInfo;
    size_t          blockSize;
} blockAllocPage_t;



bool_t BAlcAvaibleMemory( blockAllocPage_t* page ) {
    return !(page->blocksInfo == MAX_UINT64);
}

static void* BAlcAllocateBlock( blockAllocPage_t* page ) {
    uint64_t mask = M_UINT64;
    uint8_t bits = 64;
    uint8_t shift = 32;
    do {
        bits >>= 1;
        if( !(page->blocksInfo & (mask << shift)) ) {
            shift += bits;
        } else {
            
            shift -= bits;
        }
        
        mask >>= bits;
    } while( bits );
}
