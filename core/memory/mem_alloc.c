#include "mem_alloc.h"

#include <stdlib.h>

alloc_t def_alloc = { malloc, free };


/*
============
MemInit
============
*/
void MemInit( void ) {
}

/*
============
MemShutdown
============
*/
void MemShutdown( void ) {
}

/*
============
MemDefaultAlloc
============
*/
alloc_t* MemDefaultAlloc( void ) {
    return &def_alloc;
}

/*
============
MemStringAlloc
============
*/
alloc_t* MemStringAlloc( void ) {
    return MemDefaultAlloc();
}