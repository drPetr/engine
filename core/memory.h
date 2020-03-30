#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <core/types.h>

// copy memory from src to dst
void* MemCpy( void* dst, const void* src, size_t size );

// set mem to number n
void* MemSet( void* mem, int n, size_t size );

// set memory to 0
void* MemZero( void* mem, size_t size );

#endif //__MEMORY_H__