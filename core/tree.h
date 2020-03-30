#ifndef __TREE_H__
#define __TREE_H__

#include <core/types.h>



//                  data ptr, user data ptr
typedef int(*fnTreeOrder)(void*,void*);

// tree
typedef struct tree_t {
    struct tree_t*      parent;
    struct tree_t*      prev;
    struct tree_t*      next;
    struct tree_t*      firstChild;
    struct tree_t*      lastChild;
    uint8_t             data[0];
} tree_t;

#define TREE_DATA_OFFSET    ((ssize_t)(sizeof(tree_t)))



tree_t*     _TreeInsertFirstChild( tree_t* node, tree_t* insert );
// insert a subtree into the first child of tree
// function returns a pointer to the first inserted node

tree_t*     _TreeInsertLastChild( tree_t* node, tree_t* insert );
// insert a subtree into the last child of tree
// function returns a pointer to the first inserted node

tree_t*     _TreeInsertBefore( tree_t* node, tree_t* insert );
// insert a subtree before node
// function returns a pointer to the first inserted node

tree_t*     _TreeInsertAfter( tree_t* node, tree_t* insert );
// insert a subtree after node
// function returns a pointer to the first inserted node

tree_t*     _TreeInsertFirst( tree_t* node, tree_t* insert );
// insert a subtree to first
// function returns a pointer to the first inserted node

tree_t*     _TreeInsertLast( tree_t* node, tree_t* insert );
// insert a subtree to last
// function returns a pointer to the first inserted node

tree_t*     _TreeExtract( tree_t* node );
// extract a subtree from a tree
// function returns a pointer to the extracted node

tree_t*     _TreeExtractSiblings( tree_t* left, tree_t* right );
// extract sequence of siblings from a tree
// function returns a pointer to the extracted node

tree_t*     _TreeExtractChildren( tree_t* node );
// extract sequence of child
// function returns a pointer to the extracted node

int         _TreePreOrder( tree_t* node, fnTreeOrder order, void* userData );
// pre-order tree traversal
// function returns number from callback of function the order

int         _TreeInOrder( tree_t* node, fnTreeOrder order, void* userData );
// in-order tree traversal
// function returns number from callback of function the order

int         _TreePostOrder( tree_t* node, fnTreeOrder order, void* userData );
// post-order tree traversal
// function returns number from callback of function the order

void        _TreeNodeSetToNULL( tree_t* node );
// set all pointers to NULL



// aliases
#define TreeInsertFirstChild(n,i)   ((void*)_TreeInsertFirstChild(((tree_t*)(void*)(n)),(tree_t*)(void*)(i)))
#define TreeInsertLastChild(n,i)    ((void*)_TreeInsertLastChild(((tree_t*)(void*)(n)),(tree_t*)(void*)(i)))
#define TreeInsertBefore(n,i)       ((void*)_TreeInsertBefore(((tree_t*)(void*)(n)),(tree_t*)(void*)(i)))
#define TreeInsertAfter(n,i)        ((void*)_TreeInsertAfter(((tree_t*)(void*)(n)),(tree_t*)(void*)(i)))
#define TreeInsertFirst(n,i)        ((void*)_TreeInsertFirst(((tree_t*)(void*)(n)),(tree_t*)(void*)(i)))
#define TreeInsertLast(n,i)         ((void*)_TreeInsertLast(((tree_t*)(void*)(n)),(tree_t*)(void*)(i)))
#define TreeExtract(n)              ((void*)_TreeExtract((n)))
#define TreeExtractSiblings(l,r)    ((void*)_TreeExtractSiblings((l),(r)))
#define TreeExtractChildren(n)      ((void*)_TreeExtractChildren((n)))
#define TreePreOrder(n,o,u)         (_TreePreOrder((tree_t*)(void*)(n),o,(void*)(u)))
#define TreeInOrder(n,o,u)          (_TreeInOrder((tree_t*)(void*)(n),o,(void*)(u)))
#define TreePostOrder(n,o,u)        (_TreePostOrder((tree_t*)(void*)(n),o,(void*)(u)))
#define TreeNodeSetToNULL(n)        (_TreeNodeSetToNULL(((tree_t*)(void*)(n))))

#define TreeNext(n)                 ((void*)(((tree_t*)(void*)(n))->next))
#define TreePrev(n)                 ((void*)(((tree_t*)(void*)(n))->prev))
#define TreeFirstChild(n)           ((void*)(((tree_t*)(void*)(n))->firstChild))
#define TreeLastChild(n)            ((void*)(((tree_t*)(void*)(n))->lastChild))
#define TreeParent(n)               ((void*)(((tree_t*)(void*)(n))->parent))



#endif //__TREE_H__