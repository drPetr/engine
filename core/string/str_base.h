#ifndef __STR_BASE_H__
#define __STR_BASE_H__

#include <core/types.h>
#include <core/ecode.h>


int     StrCmp( const char* s1, const char* s2 );
// compare two string and return -1 if s1 is less than s2,
// or 1 if s1 is more than s2, or 0 if s1 is equal to s2

int     StrNCmp( const char* s1, const char* s2, size_t num );
// compares the first num characters of the string s1 with the first num 
// characters of the string s2, and return -1 if s1 is less than s2,
// or 1 if s1 is more than s2, or 0 if s1 is equal to s2

char*   StrCpy( char* dst, const char* src );
// copy string from src to dst

char*   StrNCpy( char* dst, const char* src, size_t size ) ;
// copy 'size' bytes the string from src to dst

size_t  StrLen( const char* str );
// return length of the string

void    StrSkipSpaces( const char** s );
// skip all spaces

bool_t  StrIsNumeric( const char* s );
// return btrue if str is int value or float value

ecode_t StrToInt( const char* s, const char** end, int* i );
// parse integer value
// return values:   E_OK, E_NUMRNG, E_NOTNUM, E_NODATA

ecode_t StrToFloat( const char* s, const char** end, float* f );
// parse float value
// return values:   E_OK, E_NOTNUM, E_NODATA



#endif //__STR_BASE_H__