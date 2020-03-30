#include <core/tree.h>
#include <core/assert.h>

/*
============
TreeInsertionHelper
============
*/
static tree_t* TreeInsertionHelper( tree_t* left, tree_t* right, tree_t* parent, tree_t* insert ) {
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
    return firstInsert;
}

/*
============
TreeExtractionHelper
============
*/
static tree_t* TreeExtractionHelper( tree_t* left, tree_t* right ) {
    tree_t* ext = NULL;
    tree_t* it;
    tree_t* parent;
    tree_t* before;
    tree_t* after;
    
    // should  be at least one node
    assert( left || right );
    
    // if left node is set and right node is set
    if( left && right ) {
        ext = left;
#ifdef DEBUG
        // check left and right sequence
        if( ext != right ) {
            while( ext && ext != right ) {
                ext = ext->next;
            }
        }
        asserta( ext == right, "error: incorrect order of parameters" );
        ext = left;
#endif
    } else if( left || right ) { // if only one node is set
        // if only one left node
        if( left ) {
            ext = left;
            right = left;
            while( right->next ) {
                right = right->next;
            }
        } else { // else right node
            left = right;
            while( left->prev ) {
                left = left->prev;
            }
            ext = left;
        }
    }
    
    parent = left->parent;
    before = left->prev;
    after = right->next;
    
    // if the left node has a previous sibling and
    // the right node has a next sibling
    if( before &&after ) {
        before->next = after;
        after->prev = before;
        left->prev = NULL;
        right->next = NULL;
    } else if( before || after ) { // else only one node has a sibling
        // if the left node has a previous sibling and
        if( before ) {
            before->next = NULL;
            left->prev = NULL;
        } else { // else the right node has a next sibling
            after->prev = NULL;
            right->next = NULL;
        }
    }
    
    // disconnect parents
    if( parent ) {
        if( left == parent->firstChild && right == parent->lastChild ) {
            // no descendants
            parent->firstChild = NULL;
            parent->lastChild = NULL;
        } else if( left == parent->firstChild || right == parent->lastChild ) {
            // set new pointers to descendants
            if( left == parent->firstChild ) {
                parent->firstChild = after;
            } else {
                parent->lastChild = before;
            }
        }
        
        it = left;
        do {
            it->parent = NULL;
            it = it->next;
        } while( it != right );
    }
    
    return ext;
}

/*
============
TreePreOrder_r
============
*/
static int TreePreOrder_r( tree_t* node, fnTreeOrder order, void* userData ) {
    int code;
    
    // exit if NULL
    if( !node ) {
        return 0;
    }
    
    // call user function
    if( (code = order( node->data, userData )) ) {
        return code;
    }
    // recursive call for first child
    if( (code = TreePreOrder_r( node->firstChild, order, userData )) ) {
        return code;
    }
    // recursive call for next node
    if( (code = TreePreOrder_r( node->next, order, userData )) ) {
        return code;
    }
    
    return 0;
}

/*
============
TreeInOrder_r
============
*/
static int TreeInOrder_r( tree_t* node, fnTreeOrder order, void* userData ) {
    int code;
    
    // exit if NULL
    if( !node ) {
        return 0;
    }
    
    // recursive call for first child
    if( (code = TreeInOrder_r( node->firstChild, order, userData )) ) {
        return code;
    }
    // call user function
    if( (code = order( node->data, userData )) ) {
        return code;
    }
    // recursive call for next node
    if( (code = TreeInOrder_r( node->next, order, userData )) ) {
        return code;
    }
    
    return 0;
}

/*
============
TreePostOrder_r
============
*/
static int TreePostOrder_r( tree_t* node, fnTreeOrder order, void* userData ) {
    int code;
    
    // exit if NULL
    if( !node ) {
        return 0;
    }
    
    // recursive call for first child
    if( (code = TreePostOrder_r( node->firstChild, order, userData )) ) {
        return code;
    }
    // recursive call for next node
    if( (code = TreePostOrder_r( node->next, order, userData )) ) {
        return code;
    }
    // call user function
    if( (code = order( node->data, userData )) ) {
        return code;
    }
    
    return 0;
}

/*
============
_TreeInsertFirstChild
============
*/
tree_t* _TreeInsertFirstChild( tree_t* node, tree_t* insert ) {
    return TreeInsertionHelper( NULL, node->firstChild, node, insert );
}

/*
============
_TreeInsertLastChild
============
*/
tree_t* _TreeInsertLastChild( tree_t* node, tree_t* insert ) {
    return TreeInsertionHelper( node->lastChild, NULL, node, insert );
}

/*
============
_TreeInsertBefore
============
*/
tree_t* _TreeInsertBefore( tree_t* node, tree_t* insert ) {
    return TreeInsertionHelper( node->prev, node, node->parent, insert );
}

/*
============
_TreeInsertAfter
============
*/
tree_t* _TreeInsertAfter( tree_t* node, tree_t* insert ) {
    return TreeInsertionHelper( node, node->next, node->parent, insert );
}

/*
============
_TreeInsertFirst
============
*/
tree_t* _TreeInsertFirst( tree_t* node, tree_t* insert ) {
    while( node->prev ) {
        node = node->prev;
    }
    return TreeInsertionHelper( NULL, node, node->parent, insert );
}

/*
============
_TreeInsertLast
============
*/
tree_t* _TreeInsertLast( tree_t* node, tree_t* insert ) {
    while( node->next ) {
        node = node->next;
    }
    return TreeInsertionHelper( node, NULL, node->parent, insert );
}

/*
============
_TreeExtract
============
*/
tree_t* _TreeExtract( tree_t* node ) {
    return TreeExtractionHelper( node, node );
}

/*
============
_TreeExtractSiblings
============
*/
tree_t* _TreeExtractSiblings( tree_t* left, tree_t* right ) {
    return TreeExtractionHelper( left, right );
}

/*
============
TreeExtractChildren
============
*/
tree_t* _TreeExtractChildren( tree_t* node ) {
    if( node && node->firstChild ) {
        return TreeExtractionHelper( node->firstChild, node->lastChild );
    }
    return NULL;
}

/*
============
_TreePreOrder
============
*/
int _TreePreOrder( tree_t* node, fnTreeOrder order, void* userData ) {
    assert_not_null( order );
    return TreePreOrder_r( node, order, userData );
}

/*
============
_TreeInOrder
============
*/
int _TreeInOrder( tree_t* node, fnTreeOrder order, void* userData ) {
    assert_not_null( order );
    return TreeInOrder_r( node, order, userData );
}

/*
============
_TreePostOrder
============
*/
int _TreePostOrder( tree_t* node, fnTreeOrder order, void* userData ) {
    assert_not_null( order );
    return TreePostOrder_r( node, order, userData );
}

/*
============
_TreeNodeSetToNULL
============
*/
void _TreeNodeSetToNULL( tree_t* node ) {
    assert_not_null( node );
    node->parent = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->firstChild = NULL;
    node->lastChild = NULL;
}
