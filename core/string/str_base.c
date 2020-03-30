#include "str_base.h"

#include <core/assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



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
StrCmp
============
*/
int StrNCmp( const char* s1, const char* s2, size_t num ) {
    return strncmp( s1, s2, num );
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
StrNCpy
============
*/
char* StrNCpy( char* dst, const char* src, size_t size ) {
    return strncpy( dst, src, size );
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
StrSkipSpaces
============
*/
void StrSkipSpaces( const char** s ) {
    if( s && *s ) {
        while( isspace( **s ) ) {
            (*s)++;
        }
    }
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
    
    // check for data
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
    } else if( *s == '+' ) {
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
                goto gorng;
            }
        } while( isdigit( *s ) );
        
        // set end ptr
        if( end ) {
            *end = s;
        }
        
        // check int range
        if( v > ((uint32_t)INT_MAX + sign) ) {
            goto gorng;
        }
        
        if( i ) {
            *i = sign ? ~v + 1 : v;           
        }
        
        return E_OK;
    }
    
    return E_NOTNUM;
    
gorng:
    // E_NUMRNG error
    if( *i ) {
        *i = sign ? INT_MIN : INT_MAX;
    }
    return E_NUMRNG;
}

/*
============
StrToFloat

return values:
E_OK        if conversion was successful
E_NOTNUM    if string is not number
E_NODATA    if string is NULL
============
*/
ecode_t StrToFloat( const char* s, const char** end, float* f ) {
    //const uint32_t FLT_MNT_MAX = 0x00800000;
    //uint32_t m = 0;     // mantissa
    //uint32_t e = 0;     // exponenta
    bool_t sign = bfalse;    // sign
    float v = 0.0f;     // value
    float d = 1.0f;
    
    // check for data
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
    } else if( *s == '+' ) {
        s++;
    }
    
    // scan number
    if( isdigit( *s ) ) {
        do {
            v = v * 10.0f + (float)((*s) - '0');
            
            // check mantissa limits
            // check int limits
            
            s++;
        } while( isdigit( *s ) );
        // skip dot
        if( *s == '.' ) {
            s++;
        }
        while( isdigit( *s ) ) {
            d *= 10.0f;
            v = v + (float)((*s) - '0') / d;
            s++;
        }
    } else {
        return E_NOTNUM;
    }
    
    // set end ptr
    if( end ) {
        *end = s;
    }
    
    // set value
    if( *f ) {
        *f = v;
        *((uint32_t*)f) = *((uint32_t*)f) | sign << 31;
    }
    
    return E_OK;
}
