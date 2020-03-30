#include <core/queue.h>

#include <core/assert.h>



/*
============
_QueuePush
============
*/
queue_t* _QueuePush( queue_t* node, queue_t* insert ) {
    assert_not_null( insert );
    
    while( insert->prev ) {
        insert = insert->prev;
    }
    
    // if node is set
    if( node ) {
        while( node->next ) {
            node = node->next;
        }
        
        // insertion
        node->next = insert;
        insert->prev = node;
    }
    
    return insert;
}

/*
============
_QueuePop
============
*/
queue_t* _QueuePop( queue_t* node ) {
    if( node ) {
        while( node->prev ) {
            node = node->prev;
        }
        if( node->next ) {
            node->next->prev = NULL;
            node->next = NULL;
        }
    }
    
    return node;
}
