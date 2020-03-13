#include <core/tree.h>
#include <core/assert.h>

/*
====================
TreeInsertionHelper
====================
*/
static void TreeInsertionHelper( tree_t* left, tree_t* right, tree_t* parent, tree_t* insert ) {
    // first node in the insert sequence
    tree_t* firstInsert = insert;
    // last node in the insert sequence
    tree_t* lastInsert = NULL;
    tree_t* tmp = insert;
    
    assert( left || right || parent );
    assert( insert );

    // set the whole sibling sequence of the parent and get a pointer to 
    // the last node of the sibling sequence
    while( tmp ) {
        lastInsert = tmp;
        tmp->parent = parent;
        tmp = tmp->next;
    }
    
    while( firstInsert->prev ) {
        firstInsert = firstInsert->prev;
    }

    // restoring "family" relations
    if( parent ) {
        if( left ) {
			left->next = firstInsert;
			firstInsert->prev = left;
		} else {
			parent->firstChild = firstInsert;
		}
		if( right ) {
			right->prev = lastInsert;
			lastInsert->next = right;
		} else {
			parent->lastChild = lastInsert;
		}
    } else {
		if( left ) {
			left->next = firstInsert;
			firstInsert->prev = left;
        }
		if( right ) {
			right->prev = lastInsert;
			lastInsert->next = right;
		}
    }
}



/*
====================
TreeNodeAllocate
====================
*/
tree_t* TreeNodeAllocate( alloc_t* alloc, size_t subsize ) {
    tree_t* node;
    node = alloc->alloc( sizeof(tree_t) + subsize );
    node->parent = NULL;
	node->next = NULL;
	node->prev = NULL;
	node->firstChild = NULL;
	node->lastChild = NULL;
    node->flags = 0;
    
    return node;
}

/*
============
TreeNodeDeallocate
============
*/
void TreeNodeDeallocate( alloc_t* alloc, tree_t* node ) {
    alloc->dealloc( node );
}

/*
====================
TreeInsertFirstChild

Вставить узел insert первым дочерним в node
====================
*/
void TreeInsertFirstChild( tree_t* node, tree_t* insert ) {
    TreeInsertionHelper( NULL, node->firstChild, node, insert );
}

/*
====================
TreeInsertLastChild

Вставить узел insert последним дочерним в node
====================
*/
void TreeInsertLastChild( tree_t* node, tree_t* insert ) {
    TreeInsertionHelper( node->lastChild, NULL, node, insert );
}

/*
====================
TreeInsertBefore

Вставить узел insert перед узлом node
====================
*/
void TreeInsertBefore( tree_t* node, tree_t* insert ) {
    TreeInsertionHelper( node->prev, node, node->parent, insert );
}

/*
====================
TreeInsertAfter

Вставить узел insert после узла node
====================
*/
void TreeInsertAfter( tree_t* node, tree_t* insert ) {
    TreeInsertionHelper( node, node->next, node->parent, insert );
}

/*
====================
TreeInsertFirst

Вставить узел insert первым в последовательности node
====================
*/
void TreeInsertFirst( tree_t* node, tree_t* insert ) {
    while( node->prev ) {
        node = node->prev;
    }
    TreeInsertionHelper( NULL, node, node->parent, insert );
}

/*
====================
TreeInsertLast

Вставить узел insert последним в последовательности node
====================
*/
void TreeInsertLast( tree_t* node, tree_t* insert ) {
    while( node->next ) {
        node = node->next;
    }
    TreeInsertionHelper( node, NULL, node->parent, insert );
}
