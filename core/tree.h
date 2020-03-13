#ifndef __TREE_H__
#define __TREE_H__

#include <core/types.h>
#include <core/memory.h>

typedef struct tree_t {
    struct tree_t*      parent;
    struct tree_t*      next;
    struct tree_t*      prev;
    struct tree_t*      firstChild;
    struct tree_t*      lastChild;
    uint32_t            flags;
    uint8_t             data[0];
} tree_t;



tree_t* TreeNodeAllocate( alloc_t* alloc, size_t subsize );
void    TreeNodeDeallocate( alloc_t* alloc, tree_t* node );
void    TreeInsertFirstChild( tree_t* node, tree_t* insert );
void    TreeInsertLastChild( tree_t* node, tree_t* insert );
void    TreeInsertBefore( tree_t* node, tree_t* insert );
void    TreeInsertAfter( tree_t* node, tree_t* insert );
void    TreeInsertFirst( tree_t* node, tree_t* insert );
void    TreeInsertLast( tree_t* node, tree_t* insert );


#endif //__TREE_H__