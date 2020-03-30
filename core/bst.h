#ifndef __BBST_H__
#define __BBST_H__
// balanced binary search tree header

#include <core/types.h>

//https://habr.com/ru/company/otus/blog/472040/
//https://ru.wikipedia.org/wiki/%D0%9A%D1%80%D0%B0%D1%81%D0%BD%D0%BE-%D1%87%D1%91%D1%80%D0%BD%D0%BE%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE
//https://neerc.ifmo.ru/wiki/index.php?title=%D0%9A%D1%80%D0%B0%D1%81%D0%BD%D0%BE-%D1%87%D0%B5%D1%80%D0%BD%D0%BE%D0%B5_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE#.D0.9E.D1.80.D0.B8.D0.B3.D0.B8.D0.BD.D0.B0.D0.BB.D1.8C.D0.BD.D1.8B.D0.B5
//https://neerc.ifmo.ru/wiki/index.php?title=%D0%94%D0%B8%D0%BD%D0%B0%D0%BC%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%B0%D1%8F_%D0%B2%D1%8B%D0%BF%D1%83%D0%BA%D0%BB%D0%B0%D1%8F_%D0%BE%D0%B1%D0%BE%D0%BB%D0%BE%D1%87%D0%BA%D0%B0_(%D0%B4%D0%BE%D1%81%D1%82%D0%B0%D1%82%D0%BE%D1%87%D0%BD%D0%BE_log%5E2_%D0%BD%D0%B0_%D0%B4%D0%BE%D0%B1%D0%B0%D0%B2%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5/%D1%83%D0%B4%D0%B0%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5)


#define BBST_RED        0x02

// data comparison function
//                      node data, insert data
typedef int(*fnBstCmp)(const void*,const void*);

typedef struct bbst_t {
    struct bbst_t*  parent;
    struct bbst_t*  left;
    struct bbst_t*  right;
    uint8_t         flags;
    uint8_t         varU8;
    uint16_t        varU16;
    uint32_t        varU32;
    uint8_t         data[0];
} bbst_t;


// red black tree
typedef struct rbtree_t {
    struct rbtree_t*    parent;
    struct rbtree_t*    parent;
} rbtree_t;



void    _BBSTInsert( bbst_t* node, bbst_t* insert, fnBstCmp cmp );
// insert a subtree into a tree

bbst_t* _BBSTExtract( bbst_t* node, fnBstCmp cmp );
// extract a subtree from a tree

bbst_t* _BBSTExtractNode( bbst_t* node, fnBstCmp cmp );
// extract only one node

bbst_t* _BBSTFind( bbst_t* root, const void* userData, fnBstCmp cmp );

void    _BBSTNodeSetToNULL( bbst_t* node );




/*
void    _TreePreOrder( tree_t* node, fnTreeOrder order, void* userData );
void    _TreeInOrder( tree_t* node, fnTreeOrder order, void* userData );
void    _TreePostOrder( tree_t* node, fnTreeOrder order, void* userData );
*/

#define BBSTLeft(n)                 ((void*)(((bbst_t*)(void*)(n))->left))
#define BBSTRight(n)                ((void*)(((bbst_t*)(void*)(n))->right))
#define BBSTParent(n)               ((void*)(((bbst_t*)(void*)(n))->parent))
#define BBSTIsRed(n)                (!!(((bbst_t*)(void*)(n))->flags | BBST_RED))
#define BBSTSetFlag(n,f)            ((((bbst_t*)(void*)(n))->flags |= BBST_RED))
#define BBSTUnsetFlag(n,f)          ((((bbst_t*)(void*)(n))->flags &= ~BBST_RED))
#define BBSTVarU8(n)                (((bbst_t*)(void*)(n))->varU8)
#define BBSTVarU16(n)               (((bbst_t*)(void*)(n))->varU16)
#define BBSTVarU32(n)               (((bbst_t*)(void*)(n))->varU32)

/*
#define TreeInsertFirstChild(n,i)   _TreeInsertFirstChild(((tree_t*)(void*)(n)),(tree_t*)(void*)(i))
#define TreeInsertLastChild(n,i)    _TreeInsertLastChild(((tree_t*)(void*)(n)),(tree_t*)(void*)(i))
#define TreeInsertBefore(n,i)       _TreeInsertBefore(((tree_t*)(void*)(n)),(tree_t*)(void*)(i))
#define TreeInsertAfter(n,i)        _TreeInsertAfter(((tree_t*)(void*)(n)),(tree_t*)(void*)(i))
#define TreeInsertFirst(n,i)        _TreeInsertFirst(((tree_t*)(void*)(n)),(tree_t*)(void*)(i))
#define TreeInsertLast(n,i)         _TreeInsertLast(((tree_t*)(void*)(n)),(tree_t*)(void*)(i))

#define TreeExtract(n)              ((void*)_TreeExtract((n)))
#define TreeExtractSiblings(l,r)    ((void*)_TreeExtractSiblings((l),(r)))
#define TreeExtractChildren(n)      ((void*)_TreeExtractChildren((n)))

#define TreeNext(n)                 ((void*)(((tree_t*)(void*)(n))->next))
#define TreePrev(n)                 ((void*)(((tree_t*)(void*)(n))->prev))
#define TreeFirstChild(n)           ((void*)(((tree_t*)(void*)(n))->firstChild))
#define TreeLastChild(n)            ((void*)(((tree_t*)(void*)(n))->lastChild))
#define TreeParent(n)               ((void*)(((tree_t*)(void*)(n))->parent))

#define TreeNodeSetToNULL(n)        _TreeNodeSetToNULL(((tree_t*)(void*)(n)))



void    TreeRelease( tree_t* root, alloc_t* alloc, fnTreeDataRelease dataRelease, void* userData );
*/

#endif //__BBST_H__