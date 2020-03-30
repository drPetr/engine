#include <core/list.h>

#include <core/assert.h>



/*
============
_ListInsertLast
============
*/
list_t* _ListInsertLast( list_t* node, list_t* insert ) {
    list_t* first;
    list_t* end;
    
    assert_not_null( insert );
    
    // first
    first = insert;
    while( first->prev ) {
        first = first->prev;
    }
    
    // if node is set
    if( node ) {
        // find last node in list
        end = node;
        while( end->next ) {
            end = end->next;
        }
        
        // insertion
        end->next = first;
        first->prev = end;
    }
    
    return first;
}

/*
============
_ListInsertFirst
============
*/
list_t* _ListInsertFirst( list_t* node, list_t* insert ) {
    list_t* first;
    list_t* last;
    list_t* begin;
    
    assert_not_null( insert );
    
    // last and first
    first = insert;
    while( first->prev ) {
        first = first->prev;
    }
    
    // if node is set
    if( node ) {
        last = insert;
        while( last->next ) {
            last = last->next;
        }
        
        // find first node in list
        begin = node;
        while( begin->prev ) {
            begin = begin->prev;
        }
        
        // insertion
        last->next = begin;
        begin->prev = last;
    }
    
    return first;
}

/*
============
_ListInsertAfter
============
*/
list_t* _ListInsertAfter( list_t* node, list_t* insert ) {
    list_t* first;
    list_t* last;
    list_t* prev;
    list_t* next;
    
    assert_not_null( insert );
    
    // last and first
    first = insert;
    while( first->prev ) {
        first = first->prev;
    }
    
    // if node is set
    if( node ) {
        // previous and next nodes
        prev = node;
        next = node->next;
        
        // insertion
        prev->next = first;
        first->prev = prev;
        if( next ) {
            last = insert;
            while( last->next ) {
                last = last->next;
            }
            next->prev = last;
            last->next = next;
        }
    }
    
    return first;
}

/*
============
_ListInsertBefore
============
*/
list_t* _ListInsertBefore( list_t* node, list_t* insert ) {
    list_t* first;
    list_t* last;
    list_t* prev;
    list_t* next;
    
    assert_not_null( insert );
    
    // last and first
    first = insert;
    while( first->prev ) {
        first = first->prev;
    }
    
    // if node is set
    if( node ) {
        last = insert;
        while( last->next ) {
            last = last->next;
        }
        
        // previous and next nodes
        prev = node->prev;
        next = node;
        
        next->prev = last;
        last->next = next;
        if( prev ) {
            first->prev = prev;
            prev->next = first;
        }
    }
    
    return first;
}

/*
============
_ListExtract
============
*/
list_t* _ListExtract( list_t* node ) {
    list_t* prev;
    list_t* next;
    
    assert_not_null( node );
    
    prev = node->prev;
    next = node->next;
    
    // restore links
    if( prev && next ) {
        prev->next = next;
        next->prev = prev;
        node->prev = NULL;
        node->next = NULL;
    } else if( prev ) {
        prev->next = NULL;
        node->prev = NULL;
    } else if( next ) {
        next->prev = NULL;
        node->next = NULL;
    }
    
    return node;
}

/*
============
_ListExtractSublist
============
*/
list_t* _ListExtractSublist( list_t* left, list_t* right ) {
    list_t* prev;
    list_t* next;
    
    asserta( left || right, "error: at least one node must not be NULL" );
        
    // 
    if( left && right ) {
    } else if( left ) {
        right = left;
        while( right->next ) {
            right = right->next;
        }
    } else if( right ) {
        left = right;
        while( left->prev ) {
            left = left->prev;
        }
    }
    
    // 
    prev = left->prev;
    next = right->next;
    
    // restore links
    if( prev && next ) {
        prev->next = next;
        next->prev = prev;
        left->prev = NULL;
        right->next = NULL;
    } else if( prev ) {
        prev->next = NULL;
        left->prev = NULL;
    } else if( next ) {
        next->prev = NULL;
        right->next = NULL;
    }
    
    return left;
}

/*
============
_ListNodeSetToNULL
============
*/
void _ListNodeSetToNULL( list_t* node ) {
    node->next = NULL;
    node->prev = NULL;
}
