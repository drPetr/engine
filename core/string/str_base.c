#include "str_base.h"

#include <core/memory.h>
#include <core/assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/*
============
StrCreate
============
*/
char* StrCreate( const char* s ) {
    char* ret;
    assert( s );
    ret = MemStringAlloc()->alloc( StrLen( s ) + 1 );
    StrCpy( ret, s );
    return ret;
}

/*
============
StrDelete
============
*/
void StrDelete( char* s ) {
    assert( s );
    MemStringAlloc()->dealloc( s );
}

/*
============
StrCmp
============
*/
int StrCmp( const char* s1, const char* s2 ) {
    return strcmp( s1, s2 );
}

/*
============
StrCpy
============
*/
char* StrCpy( char* dst, const char* src ) {
    return strcpy( dst, src );
}

/*
============
StrLen
============
*/
size_t StrLen( const char* str ) {
    return strlen( str );
}

/*
============
IsNumeric
============
*/
bool_t IsNumeric( const char *s ) {
	if ( *s == '-' ) {
		s++;
	}
    
    if( isdigit(*s) ) {
        s++;
        while( isdigit( *s ) ) {
            s++;
        }
        // check for dot
        if( *s && *s == '.' ) {
            s++;
        }
        while( isdigit( *s ) ) {
            s++;
        }
    } else {
        return bfalse;
    }

    return !*s;
}

/*
============
StrIsInt
============
*/
bool_t StrIsInt( const char* s ) {
    if ( *s == '-' ) {
		s++;
	}
    
    if( isdigit(*s) ) {
        s++;
        while( isdigit( *s ) ) {
            s++;
        }
    } else {
        return bfalse;
    }
    
    return !*s;
}

/*
============
StrIsFloat
============
*/
bool_t StrIsFloat( const char* s ) {
    if ( *s == '-' ) {
		s++;
	}
    
    if( isdigit(*s) ) {
        s++;
        while( isdigit( *s ) ) {
            s++;
        }
        // check for dot
        if( *s && *s == '.' ) {
            s++;
        } else {
            return bfalse;
        }
        while( isdigit( *s ) ) {
            s++;
        }
    } else {
        return bfalse;
    }

    return !*s;
}

/*
============
StrToInt
============
*/
int StrToInt( const char* s ) {
    return atoi( s );
}

/*
============
StrToFloat
============
*/
float StrToFloat( const char* s ) {
    return atof( s );
}