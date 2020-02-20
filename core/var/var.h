#ifndef __base_VAR_H__
#define __base_VAR_H__

#include <core/types.h>

/*
================================================================

VAR - global variable system

================================================================
*/

enum varFlags_t {
    VAR_TYPE_INT = 1,
    VAR_TYPE_FLOAT = 2,
    VAR_TYPE_STRING = 3,
    VAR_TYPE_MASK = 0x0f
};

typedef struct var_t {
    char*           name;
    char*           descr;
    char*           s;
    int             i;
    float           f;
    uint32_t        flags;
} var_t;



void            VarInit( void );
void            VarShutdown( void );


var_t*          VarCreate( const char* name, const char* val, uint32_t flags );
var_t*          VarSet( const char* name, const char* val );
var_t*          VarGet( const char* name );
bool_t          VarDelete( const char* name );


#endif //__base_VAR_H__