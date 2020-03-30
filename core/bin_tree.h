#ifndef __BIN_TREE_H__
#define __BIN_TREE_H__

#include <core/types.h>



//                      data ptr, user data ptr
typedef int(*fnBinTreeOrder)(void*,void*);

// binary tree
typedef struct binTree_t {
    struct binTree_t*   parent;
    struct binTree_t*   left;
    struct binTree_t*   right;
    uint8_t             data[0];
} binTree_t;

#define BIN_TREE_DATA_OFFSET    ((ssize_t)(sizeof(binTree_t)))



binTree_t*  _BinTreeInsertLeft( binTree_t* node, binTree_t* insert );
// insert a subtree into a tree
// function returns a pointer to the inserted node

binTree_t*  _BinTreeInsertRight( binTree_t* node, binTree_t* insert );
// insert a subtree into a tree
// function returns a pointer to the inserted node

binTree_t*  _BinTreeExtract( binTree_t* node );
// extract a subtree from a tree
// function returns a pointer to the extracted node

int         _BinTreePreOrder( binTree_t* node, fnBinTreeOrder order, void* userData );
// pre-order tree traversal
// function returns number from callback of function the order

int         _BinTreeInOrder( binTree_t* node, fnBinTreeOrder order, void* userData );
// in-order tree traversal
// function returns number from callback of function the order

int         _BinTreePostOrder( binTree_t* node, fnBinTreeOrder order, void* userData );
// post-order tree traversal
// function returns number from callback of function the order

void        _BinTreeNodeSetToNULL( binTree_t* node );
// set all pointers to NULL



// aliases
#define     BinTreeInsertLeft(n,i)  ((void*)_BinTreeInsertLeft(((binTree_t*)(void*)(n)),(binTree_t*)(void*)(i)))
#define     BinTreeInsertRight(n,i) ((void*)_BinTreeInsertRight(((binTree_t*)(void*)(n)),(binTree_t*)(void*)(i)))
#define     BinTreeExtract(n)       ((void*)_BinTreeExtract((binTree_t*)(void*)(n)))
#define     BinTreePreOrder(n,o,u)  (_BinTreePreOrder(((binTree_t*)(void*)(n)),o,(void*)(u)))
#define     BinTreeInOrder(n,o,u)   (_BinTreeInOrder(((binTree_t*)(void*)(n)),o,(void*)(u)))
#define     BinTreePostOrder(n,o,u) (_BinTreePostOrder(((binTree_t*)(void*)(n)),o,(void*)(u)))
#define     BinTreeNodeSetToNULL(n) (_BinTreeNodeSetToNULL((binTree_t*)(void*)(n)))

#define     BinTreeLeft(n)          ((void*)(((binTree_t*)(void*)(n))->left))
#define     BinTreeRight(n)         ((void*)(((binTree_t*)(void*)(n))->right))
#define     BinTreeParent(n)        ((void*)(((binTree_t*)(void*)(n))->parent))



#endif //__BIN_TREE_H__