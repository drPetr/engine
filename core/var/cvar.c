#include "cvar.h"

#include <core/assert.h>

#define VAR_MAX 1024

var_t vars_g[VAR_MAX];


/*
============
VarClear
============
*/
static void VarClear( vars_g* v ) {
    v->name = NULL;
    v->descr = NULL;
    v->s = NULL;
    v->i = NULL;
    v->f = NULL;
    v->flags = 0;
}

/*
============
VarIsEmpty
============
*/
static bool_t VarIsEmpty( vars_g* v ) {
    if( v->flags == 0 ) {
        return btrue;
    }
    return bfalse;
}

/*
============
VarGetEmpty
============
*/
static var_t* VarGetEmpty( void ) {
    int i = 0;
    for( ; i < VAR_MAX; i++ ) {
        if( VarIsEmpty( &(vars_g[ i ]) ) ) {
            return &(vars_g[ i ]);
        }
    }
    return NULL;
}



/*
============
VarInit
============
*/
void VarInit( void ) {
    int i;
    for( i = 0; i < VAR_MAX; i++ ) {
        VarClear( &(vars_g[ i ]) );
    }
}

/*
============
VarShutdown
============
*/
void VarShutdown( void ) {
    int i;
    for( i = 0; i < VAR_MAX; i++ ) {
        
        VarClear( &(vars_g[ i ]) );
    }
}

/*
============
VarCreate
============
*/
var_t* VarCreate( const char* name, const char* val, uint32_t flags ) {
    var_t* v;
    v = VarGetEmpty();
    asserta( v, );
}

/*
============
VarSet
============
*/
var_t* VarSet( const char* name, const char* val ) {
    
}

var_t* VarGet( const char* name ) {
    int i = 0;
    for( ; i < VAR_MAX; i++ ) {
        if( !VarIsEmpty( &(vars_g[i]) ) ) {
            if(  )
        }
    }
}

var_t* VarDelete( char* name ) {
    
}