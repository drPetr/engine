#include "var.h"

#include <core/assert.h>
#include <core/memory.h>
#include <core/string.h>

#define VAR_MAX 1024

var_t vars_g[VAR_MAX];


/*
============
VarClear
============
*/
static void VarClear( var_t* v ) {
    v->name = NULL;
    v->descr = NULL;
    v->s = NULL;
    v->i = 0;
    v->f = 0.0f;
    v->flags = 0;
}

/*
============
VarIsEmpty
============
*/
static bool_t VarIsEmpty( var_t* v ) {
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
VarDeleteAndClear
============
*/
static void VarDeleteAndClear( var_t* v ) {
    alloc_t* a = MemStringAlloc();
    
    // delete strings
    if( v->name ) {
        a->dealloc( v->name );
    }
    if( v->descr ) {
        a->dealloc( v->descr );
    }
    if( v->s ) {
        a->dealloc( v->s );
    }
    // clear value
    VarClear( v );
}

/*
============
VarCheckVarName
============
*/
static bool_t VarCheckVarName( const char* name ) {
    const char* s = name;
    
    assert( name );

    // if string is empty
    if( *s == 0 ) {
        return bfalse;
    }
    // check string
    while( *s ) {
        if( !((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') ||
            (*s >= '0' && *s <= '9') || (*s == '_') ) ) {
            return bfalse;
        }
        s++;
    }

    // string is valid
    return btrue;
}

/*
============
VarCheckType
============
*/
static bool_t VarCheckType( const char* val, uint32_t flags ) {
    switch( flags & VAR_TYPE_MASK ) {
        case VAR_TYPE_INT:
            return StrIsInt( val );
        case VAR_TYPE_FLOAT:
            return StrIsFloat( val );
        case VAR_TYPE_STRING:
            return btrue;
    }
    
    return bfalse;
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
    var_t* v;
    int i;

    for( i = 0, v = vars_g; i < VAR_MAX; i++, v++ ) {
        VarDeleteAndClear( v );
    }
}

/*
============
VarCreate
============
*/
var_t* VarCreate( const char* name, const char* val, uint32_t flags ) {
    var_t* v;

    assert( name );
    assert( val );
    
    // check variable name for valid
    if( !VarCheckVarName( name ) ) {
        return NULL;
    }

    v = VarGetEmpty();

    // verify found value
    asserta( v, "error: impossible to find free space for a variable. Increase size VAR_MAX\n" );
    
    if( !VarCheckType( val, flags ) ) {
        asserta( bfalse, "error: invalid value type\n" );
        return NULL;
    }
    
    v->name = StrCreate( name );
    v->descr = NULL;
    v->s = StrCreate( val );
    
    // convert value
    switch( flags & VAR_TYPE_MASK ) {
        case VAR_TYPE_INT:
        case VAR_TYPE_FLOAT:
            v->i = StrToInt( val );
            v->f = StrIsFloat( val );
            break;
        case VAR_TYPE_STRING:
            v->i = 0;
            v->f = 0.0f;
            break;
    }
    
    return v;
}

/*
============
VarSet
============
*/
var_t* VarSet( const char* name, const char* val ) {
    var_t* v;
    
    assert( name );
    assert( val );
    
    // find value
    v = VarGet( name );
    if( !v ) {
        return NULL;
    }
    
    // check variable type
    if( !VarCheckType( val, v->flags ) ) {
        asserta( bfalse, "error: invalid value type\n" );
        return NULL;
    }
    
    // delete old string value
    StrDelete( v->s );
    v->s = StrCreate( val );
    
    // convert value
    switch( v->flags & VAR_TYPE_MASK ) {
        case VAR_TYPE_INT:
        case VAR_TYPE_FLOAT:
            v->i = StrToInt( val );
            v->f = StrIsFloat( val );
            break;
        case VAR_TYPE_STRING:
            v->i = 0;
            v->f = 0.0f;
            break;
    }
    
    return v;
}

/*
============
VarGet
============
*/
var_t* VarGet( const char* name ) {
    var_t* v;
    int i;
    
    assert( name );
    
    // find variable by name
    for( i = 0, v = vars_g; i < VAR_MAX; i++, v++ ) {
        if( !VarIsEmpty( v ) ) {
            if( !StrCmp( v->name, name ) ) {
                // return pointer to the variable
                return v;
            }
        }
    }
    
    // variable not found
    return NULL;
}

/*
============
VarDelete
============
*/
bool_t VarDelete( const char* name ) {
    var_t* v;
    
    assert( name );
    v = VarGet( name );
    if( v ) {
        VarDeleteAndClear( v );
        return btrue;
    }
    return bfalse;
}