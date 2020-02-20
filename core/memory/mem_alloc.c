#include "mem_alloc.h"

#include <stdlib.h>

alloc_t def_alloc = { malloc, free };


/*
================
MemInit
================
*/
void MemInit( void ) {
}

/*
================
MemRelease
================
*/
void MemRelease( void ) {
}

/*
============
MemDefaultAlloc
============
*/
alloc_t* MemDefaultAlloc( void ) {
    return &def_alloc;
}

alloc_t* MemStringAlloc( void ) {
    return MemDefaultAlloc();
}