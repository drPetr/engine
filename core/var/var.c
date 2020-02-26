#include <core/var.h>

#include <core/assert.h>
#include <core/memory.h>
#include <core/string.h>
#include <core/math.h>

#define VAR_LIM_DELIM   ';'
#define VAR_MAX 128

var_t vars_g[VAR_MAX];



/*
============
CreateVarString
============
*/
static char* CreateVarString( const char* s ) {
    char* str;
    str = (char*)MemStringAlloc()->alloc( StrLen(s) + 1 );
    StrCpy( str, s );
    return str;
}

/*
============
DeleteVarString
============
*/
static void DeleteVarString( char* s ) {
    MemStringAlloc()->dealloc( s );
}

/*
============
ClearVar
============
*/
static void ClearVar( var_t* v ) {
    v->name = NULL;
    v->descr = NULL;
    v->lim = NULL;
    v->flags = 0;
    v->upd = 0;
    v->i = 0;
    v->imin = 0;
    v->imax = 0;
}

/*
============
VarIsEmpty
============
*/
static bool_t VarIsEmpty( var_t* v ) {
    return v->flags == 0 && v->upd == 0;
}

/*
============
CheckVarType
============
*/
static bool_t CheckVarType( uint32_t flags ) {
    uint32_t type = flags & VAR_TYPE_MASK;
    return  type == VAR_INT || type == VAR_FLT || type == VAR_STR;
}

/*
============
GetEmptyVar
============
*/
static var_t* GetEmptyVar( void ) {
    ssize_t i;
    for( i = 0; i < VAR_MAX; i++ ) {
        if( VarIsEmpty( vars_g + i ) ) {
            return vars_g + i;
        }
    }
    return NULL;
}

/*
============
CheckVarName
============
*/
bool_t CheckVarName( const char* s ) {
    if( (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || (*s == '_') ) {
        s++;
        while( (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') ||
               (*s >= '0' && *s <= '9') || (*s == '_') ) {
            s++;
        }
        if( *s == 0 ) {
            return btrue;
        }
    }
    return bfalse;
}





/*
============
VarInit
============
*/
void VarInit( void ) {
    ssize_t i;
    for( i = 0; i < VAR_MAX; i++ ) {
        ClearVar( vars_g + i );
    }
}

/*
============
VarShutdown
============
*/
void VarShutdown( void ) {
    ssize_t i;
    var_t* v = vars_g;
    for( i = 0; i < VAR_MAX; i++, v++ ) {
        if( !VarIsEmpty( v ) ) {
            VarDelete( v );
        }
    }
}

/*
============
VarCreate
============
*/
var_t* VarCreate( const char* name, uint32_t flags, const char* val, const char* lim, const char* descr ) {
    var_t* v;
    
    assert( name );
    assert( val );
    assert( CheckVarType( flags ) );
    v = GetEmptyVar();
    assert( v );
    
    if( !CheckVarName( name ) ) {
        return NULL;
    }
    if( VarFind( name ) ) {
        return NULL;
    }
    
    v->flags = flags;
    v->upd = 1;
    VarUpdateDescr( v, descr );
    if( !(VarUpdateLim( v, lim ) && VarUpdate( v, val )) ) {
        VarDelete( v );
        return NULL;
    }
    v->name = CreateVarString( name );
    
    return v;
}

/*
============
VarFind
============
*/
var_t* VarFind( const char* name ) {
    ssize_t i;
    assert( name );
    
    // find variable by name
    var_t* v = vars_g;
    for( i = 0; i < VAR_MAX; i++, v++ ) {
        if( !VarIsEmpty( v ) && (StrCmp( v->name, name ) == 0) ) {
            return v;
        }
    }
    // variavle not found
    return NULL;
}

/*
============
VarType
============
*/
uint32_t VarType( const var_t* v ) {
    assert( v );
    return v->flags & VAR_TYPE_MASK;
}

/*
============
VarUpdate
============
*/
bool_t VarUpdate( var_t* v, const char* val ) {
    int i;
    float f;
    
    assert( v );
    assert( val );
    assert( CheckVarType( v->flags ) );
    
    switch( VarType( v ) ) {
        case VAR_INT:
            // parse int value
            if( E_OK != StrToInt( val, &val, &i ) ) {
                return bfalse;
            }
            StrSkipSpaces( &val );
            if( *val != 0 ) {
                return bfalse;
            }
            
            // clamping int value
            if( i > v->imax ) {
                v->i = v->imax;
            } else if( i < v->imin ) {
                v->i = v->imin;
            } else {
                v->i = i;
            }
            
            break;
        case VAR_FLT:
            // parse float value
            if( E_OK != StrToFloat( val, &val, &f ) ) {
                return bfalse;
            }
            StrSkipSpaces( &val );
            if( *val != 0 ) {
                return bfalse;
            }
            
            // clamping int value
            if( f > v->fmax ) {
                v->f = v->fmax;
            } else if( f < v->fmin ) {
                v->f = v->fmin;
            } else {
                v->f = f;
            }
        
            break;
        case VAR_STR:
            // update lim string
            if( v->s ) {
                DeleteVarString( v->s );
            }
            v->s = val ? CreateVarString( val ) : NULL;

            break;
    }
    
    v->upd++;
    
    return btrue;
}

/*
============
VarUpdateLim
============
*/
bool_t VarUpdateLim( var_t* v, const char* lim ) {
    uint32_t type;
    float fmin;
    float fmax;
    int imin;
    int imax;
    
    assert( v );
    assert( CheckVarType( v->flags ) );
    
    type = VarType( v );
    // parse limits
    switch( type ) {
        case VAR_INT:
            if( lim ) {
                // parse int limits
                if( E_OK != StrToInt( lim, &lim, &imin ) ) {
                    return bfalse;
                }
                StrSkipSpaces( &lim );
                if( *lim != VAR_LIM_DELIM ) {
                    return bfalse;
                }
                lim++;
                if( E_OK != StrToInt( lim, &lim, &imax ) ) {
                    return bfalse;
                }
                StrSkipSpaces( &lim );
                if( *lim != 0 ) {
                    return bfalse;
                }

                // check int limits
                if( imin > imax ) {
                    return bfalse;
                }

                v->imin = imin;
                v->imax = imax;
            } else {
                v->imin = INT_MIN;
                v->imax = INT_MAX;
            }
            
            // clamp int value
            if( v->i < v->imin ) {
                v->i = v->imin;
                v->upd++;
            }
            if( v->i > v->imax ) {
                v->i = v->imax;
                v->upd++;
            }
            
            break;
        case VAR_FLT:
            if( lim ) {
                // parse float limits
                if( E_OK != StrToFloat( lim, &lim, &fmin ) ) {
                    return bfalse;
                }
                StrSkipSpaces( &lim );
                if( *lim != VAR_LIM_DELIM ) {
                    return bfalse;
                }
                lim++;
                if( E_OK != StrToFloat( lim, &lim, &fmax ) ) {
                    return bfalse;
                }
                StrSkipSpaces( &lim );
                if( *lim != 0 ) {
                    return bfalse;
                }
                
                // check float limits
                if( fmin > fmax ) {
                    return bfalse;
                }

                v->fmin = fmin;
                v->fmax = fmax;
            } else {
                v->fmin = FLT_INFINITY; 
                v->fmax = -FLT_INFINITY;
            }
            
            // clamp float value
            if( v->f < v->fmin ) {
                v->f = v->fmin;
                v->upd++;
            }
            if( v->f > v->fmax ) {
                v->f = v->fmax;
                v->upd++;
            }
            
            break;
        case VAR_STR:
            return btrue;
    }
    
    // update lim string
    if( v->lim ) {
        DeleteVarString( v->lim );
    }
    v->lim = lim ? CreateVarString( lim ) : NULL;
    
    return btrue;
}

/*
============
VarUpdateDescr
============
*/
bool_t VarUpdateDescr( var_t* v, const char* descr ) {
    assert( v );

    // check for empty
    if( VarIsEmpty( v ) ) {
        return bfalse;
    }

    // update var description
    if( v->descr ) {
        DeleteVarString( v->descr );
    }
    v->descr = descr ? CreateVarString( descr ) : NULL;

    return btrue;
}

/*
============
VarDelete
============
*/
void VarDelete( var_t* v ) {
    assert( v );
    assert( !VarIsEmpty( v ) );
    
    // delete strings
    if( v->name ) {
        DeleteVarString( v->name );
    }
    if( v->descr ) {
        DeleteVarString( v->descr );
    }
    if( v->lim ) {
        DeleteVarString( v->lim );
    }
    if( VarType( v ) == VAR_STR ) {
        DeleteVarString( v->s );
    }
    ClearVar( v );
}



