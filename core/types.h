#ifndef __base_TYPES_H__
#define __base_TYPES_H__


/*
included for next types:

int8_t  int16_t  int32_t  int64_t
uint8_t uint16_t uint32_t uint64_t
*/
#include <stdint.h>
#include <limits.h>

/*
included for next types:

size_t  ptrdiff_t ssize_t wchar_t
*/
#include <stddef.h>


typedef unsigned char       bool_t;

#define btrue               ((bool_t)1)
#define bfalse              ((bool_t)0)


#endif //__base_TYPES_H__