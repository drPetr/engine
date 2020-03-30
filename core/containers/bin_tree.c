#include <core/bin_tree.h>
#include <core/assert.h>



/*
============
BinTreePreOrder_r
============
*/
static int BinTreePreOrder_r( binTree_t* node, fnBinTreeOrder order, void* userData ) {
    int code;
    
    // exit if NULL
    if( !node ) {
        return 0;
    }
    
    // call user function
    if( (code = order( node->data, userData )) ) {
        return code;
    }
    // recursive call for left subtree
    if( (code = BinTreePreOrder_r( node->left, order, userData )) ) {
        return code;
    }
    // recursive call for right subtree
    if( (code = BinTreePreOrder_r( node->right, order, userData )) ) {
        return code;
    }
    
    return 0;
}

/*
============
BinTreeInOrder_r
============
*/
static int BinTreeInOrder_r( binTree_t* node, fnBinTreeOrder order, void* userData ) {
    int code;
    
    // exit if NULL
    if( !node ) {
        return 0;
    }
    
    // recursive call for left subtree
    if( (code = BinTreeInOrder_r( node->left, order, userData )) ) {
        return code;
    }
    // call user function
    if( (code = order( node->data, userData )) ) {
        return code;
    }
    // recursive call for right subtree
    if( (code = BinTreeInOrder_r( node->right, order, userData )) ) {
        return code;
    }
    
    return 0;
}

/*
============
BinTreePostOrder_r
============
*/
static int BinTreePostOrder_r( binTree_t* node, fnBinTreeOrder order, void* userData ) {
    int code;
    
    // exit if NULL
    if( !node ) {
        return 0;
    }
    
    // recursive call for left subtree
    if( (code = BinTreePostOrder_r( node->left, order, userData )) ) {
        return code;
    }
    // recursive call for right subtree
    if( (code = BinTreePostOrder_r( node->right, order, userData )) ) {
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
_BinTreeInsertLeft
============
*/
binTree_t* _BinTreeInsertLeft( binTree_t* node, binTree_t* insert ) {
    // assertion checks
    assert_not_null( node );
    assert_not_null( insert );
    assert_is_null( insert->parent );
    assert_is_null( node->left );
    
    node->left = insert;
    insert->parent = node;
    return insert;
}

/*
============
_BinTreeInsertRight
============
*/
binTree_t* _BinTreeInsertRight( binTree_t* node, binTree_t* insert ) {
    // assertion checks
    assert_not_null( node );
    assert_not_null( insert );
    assert_is_null( insert->parent );
    assert_is_null( node->right );
    
    node->right = insert;
    insert->parent = node;
    return insert;
}

/*
============
_BinTreeExtract
============
*/
binTree_t* _BinTreeExtract( binTree_t* node ) {
    binTree_t* parent;
    
    // assertion checks
    assert_not_null( node );
    
    parent = node->parent;
    if( parent ) {
        if( parent->left == node ) {
            // extract from left subtree
            parent->left = NULL;
        } else {
            // extract from right subtree
            parent->right = NULL;
        }
        node->parent = NULL;
        
        // assertion checks
        asserta( parent->left == node || parent->right == node, "check for correct pointers" );
    }
    
    return node;
}

/*
============
_BinTreePreOrder
============
*/
int _BinTreePreOrder( binTree_t* node, fnBinTreeOrder oreder, void* userData ) {
    assert_not_null( oreder );
    return BinTreePreOrder_r( node, oreder, userData );
}

/*
============
_BinTreeInOrder
============
*/
int _BinTreeInOrder( binTree_t* node, fnBinTreeOrder oreder, void* userData ) {
    assert_not_null( oreder );
    return BinTreeInOrder_r( node, oreder, userData );
}

/*
============
_BinTreePostOrder
============
*/
int _BinTreePostOrder( binTree_t* node, fnBinTreeOrder oreder, void* userData ) {
    assert_not_null( oreder );
    return BinTreePostOrder_r( node, oreder, userData );
}

/*
============
_BinTreeNodeSetToNULL
============
*/
void _BinTreeNodeSetToNULL( binTree_t* node ) {
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
}

