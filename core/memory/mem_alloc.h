#ifndef __MEM_ALLOC_H__
#define __MEM_ALLOC_H__

#include <stddef.h> // for size_t


typedef void*(*fnAlloc)(size_t);
typedef void(*fnDealloc)(void*);

typedef struct {
    fnAlloc         alloc;
    fnDealloc       dealloc;
} alloc_t;



void        MemInit( void );
// initialize memory

void        MemShutdown( void );
// finalize memory



alloc_t*    MemDefaultAlloc( void );
// get pointer to the default allocator

alloc_t*    MemStringAlloc( void );
// get pointer to the string allocator



#endif //__MEM_ALLOC_H__