#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <core/types.h>

// function allocator
typedef void*(*fnAllocate)(size_t);
// deallocator function
typedef void(*fnDeallocate)(void*);


typedef struct {
    fnAllocate      allocate;
    fnDeallocate    deallocate; // return (allocated memory) to the store of available RAM.
} alloc_t;



#define alloc(a,mem)     ((a)->allocate(mem))
#define dealloc(a,ptr)   ((a)->deallocate(ptr))


void    AllocInit( void );
void    AllocShutdown( void );

alloc_t*    GetVarStringsAllocator( void );

alloc_t*    GetArchiveFileNamesStringAllocator( void );

alloc_t*    GetSystemAllocator( void );
// used to allocate system memory. That is, this memory will 
// be allocated in the INITIAL functions and freed in 
// the SHUTDOWN functions

alloc_t*    GetFsTreeAllocator( void );
alloc_t*    GetFsTreeStringsAllocator( void );

alloc_t*    GetZipFileStructAllocator( void );




#endif //__ALLOC_H__