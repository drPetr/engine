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
void        MemRelease( void );

alloc_t*    MemDefaultAlloc( void );
alloc_t*    MemStringAlloc( void );



#endif //__MEM_ALLOC_H__