#include <core/bst.h>



/*
============
BBSTBalanceCaseRedUncle_r

case 1
if parent and uncle are red color
============
*/
static void BBSTBalanceCaseRedUncle_r( bbst_t* node ) {
    bbst_t* parent;
    bbst_t* grandParent;
    bbst_t* uncle;
    
    // check for grandparent
    if( !(node->parent) || !(node->parent->parent) ) {
        return;
    }
    
    parent = node->parent;
    grandParent = parent->parent;
    uncle = parent == grandParent->left ? grandParent->right : grandParent->left;
    
    // if the parent and uncle are red, then we make a balancing
    if( BBSTIsRed( parent ) && BBSTIsRed( uncle ) ) {
        BBSTSetFlag( parent, BBST_RED );            // make red
        BBSTSetFlag( uncle, BBST_RED );             // make red
        BBSTUnsetFlag( grandParent, BBST_RED );     // make black
        BBSTBalanceCaseRedUncle_r( grandParent );
    }
}

/*
============
BBSTLeftRotate
============
*/
static void BBSTLeftRotate( bbst_t* node ) {
    /**************************
    *  rotate node x to left *
    **************************/

    bbst_t* l = BBSTleft( node );
    bbst_t* r = BBSTRight( node );
    bbst_t* tmp;
    

    r->parent = node->parent;
    node->parent = r;
    



    Node *y = x->right;

    /* establish x->right link */
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;

    /* establish y->parent link */
    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        root = y;
    }

    /* link x and y */
    y->left = x;
    if (x != NIL) x->parent = y;
}

/*
============
BBSTRightRotate
============
*/
static void BBSTRightRotate( bbst_t* node ) {
    
}





void _BBSTInsert( bbst_t* node, bbst_t* insert, fnBstCmp cmp ) {
    bbst_t* insertTo;
    int c;
    
    while( BBSTParent( node ) ) {
        node = BBSTParent( node );
    }
    while( BBSTParent( insert ) ) {
        insert = BBSTParent( insert );
    }
    
    insertTo = node;
    do {
        c = cmp( (void*)insertTo->data, (void*)insert->data );
        if( c < 0 ) {
            if( BBSTLeft( insertTo ) ) {
                insertTo = BBSTLeft( insertTo );
            } else {
                // insert into the left node
                insertTo->left = insert;
                insert->parent = insertTo;
            }
        } else { // c >= 0
            if( BBSTRight( insertTo ) ) {
                insertTo = BBSTRight( insertTo );
            } else {
                // insert into the right node
                insertTo->right = insert;
                insert->parent = insertTo;
            }
        }
    } while( 1 );
}

/*
============
BBSTExtract
============
*/
bbst_t* _BBSTExtract( bbst_t* node, fnBstCmp cmp ) {
    
}

/*
============
BBSTFind
============
*/
bbst_t* _BBSTFind( bbst_t* root, const void* userData, fnBstCmp cmp ) {
    int c;
    
    while( BBSTParent( root ) ) {
        root = BBSTParent( root );
    }
    
    do {
        c = cmp( root->data );
        BBSTParent( root )
    } while( 1 );
}

/*
============
BBSTNodeSetToNULL
============
*/
void _BBSTNodeSetToNULL( bbst_t* node ) {
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->flags = 0;
}
