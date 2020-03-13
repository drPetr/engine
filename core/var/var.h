#ifndef __base_VAR_H__
#define __base_VAR_H__

#include <core/types.h>

/*
================================================================

VAR - global variable system

================================================================
*/

enum varFlags_t {
    VAR_INT = 1,
    VAR_FLT = 2,
    VAR_STR = 3,
    VAR_BOOL = 4,
    
    
    VAR_TYPE_MASK = 0x0f
};



typedef struct var_t {
    char*           name;
    char*           descr;
    char*           s;
    uint32_t        flags;
    uint32_t        update;
    union {
        struct {
            int             i;
            int             imin;
            int             imax;
        };
        struct {
            float           f;
            float           fmin;
            float           fmax;
        };
    };
} var_t;



void            VarInit( void );
void            VarShutdown( void );


var_t*          VarCreate( const char* name, const char* val, uint32_t flags, const char* limits );
var_t*          VarCreate( const char* name, const char* val, uint32_t flags );
var_t*          VarUpdate( const char* name, const char* val );
var_t*          VarFind( const char* name );
bool_t          VarDelete( const char* name );


#endif //__base_VAR_H__