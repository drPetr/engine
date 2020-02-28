#include <core/memory.h>

#include <string.h>

/*
============
MemCpy
============
*/
void* MemCpy( void* dst, const void* src, size_t size ) {
    return memcpy( dst, src, size );
}

/*
============
MemSet
============
*/
void* MemSet( void* mem, int n, size_t size ) {
    return memset( mem, n, size );
}

/*
============
MemZero
============
*/
void* MemZero( void* mem, size_t size ) {
    return MemSet( mem, 0, size );
}