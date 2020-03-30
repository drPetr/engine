#ifndef __HASH_H__
#define __HASH_H__

#include <core/types.h>

typedef uint32_t    crc32_t;

crc32_t CRC32Update( crc32_t crc, const void* data, size_t len );

#endif //__HASH_H__