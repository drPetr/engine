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
    VAR_TYPE_MASK = 0x0f
};

typedef struct var_t {
    char*           name;       // var name
    char*           descr;      // description
    char*           lim;        // limits
    uint32_t        flags;      // flags
    uint32_t        upd;        // last update counter
    union {
        struct {
            int     i;          // int value
            int     imin;       // min int value
            int     imax;       // max int value
        };
        struct {
            float   f;          // float value
            float   fmin;       // min float value
            float   fmax;       // max float value
        };
        char*       s;          // string value
    };
} var_t;



void            VarInit( void );
void            VarShutdown( void );

var_t*          VarCreate( const char* name, uint32_t flags, const char* val, const char* lim, const char* descr );
var_t*          VarFind( const char* name );
uint32_t        VarType( const var_t* v );
bool_t          VarUpdate( var_t* v, const char* val );
bool_t          VarUpdateLim( var_t* v, const char* lim );
bool_t          VarUpdateDescr( var_t* v, const char* descr );
bool_t          VarDelete( var_t* v );


#endif //__base_VAR_H__