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
*//*
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
}*/

/*
============
StrIsFloat
============
*//*
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
}*/

/*
============
StrToInt
============
*//*
int StrToInt( const char* s ) {
    return atoi( s );
}*/

/*
============
StrToFloat
============
*//*
float StrToFloat( const char* s ) {
    return atof( s );
}*/



/*
============
StrToInt

return values:
E_OK        if conversion was successful
E_NUMRNG    if the number is out of range of int [INT_MIN;INT_MAX]
E_NOTNUM    if string is not number
E_NODATA    if string is NULL
============
*/
ecode_t StrToInt( const char* s, const char** end, int* i ) {
    uint32_t v = 0;
    uint32_t prev = v;
    bool_t sign = bfalse;
    
    if( !s ) {
        return E_NODATA;
    }
    
    // skip spaces
    while( isspace( *s ) ) {
        s++;
    }
    
    // skip sign
    if( *s == '-' ) {
        sign = btrue;
        s++;
    }
    
    if( isdigit( *s ) ) {
        // scaning number
        do {
            prev = v;
            v = v * 10 + ((*s) - '0');
            s++;
            // check int limits
            if( prev > v ) {
                if( end ) {
                    while( isdigit( *s ) ) {
                        s++;
                    }
                    *end = s;
                }
                return E_NUMRNG;
            }
        } while( isdigit( *s ) );
        
        // set end ptr
        if( end ) {
            *end = s;
        }
        
        // check int range
        if( v > ((uint32_t)INT_MAX + sign) ) {
            return E_NUMRNG;
        }
        
        if( i ) {
            *i = sign ? ~v + 1 : v;           
        }
        
        return E_OK;
    }
    
    return E_NOTNUM;
}

/*
============
StrToFloat
============
*/
ecode_t StrToFloat( const char* s, const char** end, float* f ) {
    
    return E_NOTNUM;
}
