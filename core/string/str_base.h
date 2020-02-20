#ifndef __STR_BASE_H__
#define __STR_BASE_H__

#include <core/types.h>


char*   StrCreate( const char* s );
// allocate memory for string and copy string

void    StrDelete( char* s );
// deallocate string memory and delete string

int     StrCmp( const char* s1, const char* s2 );
// compare two string and return -1 if s1 is less than s2,
// or 1 if s1 is more than s2, or 0 if s1 is equal to s2

char*   StrCpy( char* dst, const char* src );
// copy string from src to dst

size_t  StrLen( const char* str );
// return length of the string

bool_t  StrIsNumeric( const char* s );
// return btrue if str is int value or float value

bool_t  StrIsInt( const char* s );
// return btrue if str is int value

bool_t  StrIsFloat( const char* s );
// return bture if str is float value

int     StrToInt( const char* s );
// parse integer value

float   StrToFloat( const char* s );
// parse float value


#endif //__STR_BASE_H__