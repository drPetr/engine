#ifndef __CVAR_H__
#define __CVAR_H__

#include <core/types.h>



enum varFlags_t {
    VAR_TYPE_INT = 1,
    VAR_TYPE_FLOAT = 2,
    VAR_TYPE_STRING = 4,
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


var_t*          VarCreate( char* name, char* val, uint32_t flags );
var_t*          VarSet( char* name, char* val );
var_t*          VarGet( char* name );
var_t*          VarDelete( char* name );


#endif //__CVAR_H__