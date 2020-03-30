#ifndef __LIST_H__
#define __LIST_H__

#include <core/types.h>


//                  data ptr, user data ptr
typedef int(*fnListOrder)(void*,void*);

// singly linked list
typedef struct list_t {
    struct list_t*  next;
    struct list_t*  prev;
    uint8_t         data[0];
} list_t;

#define LIST_DATA_OFFSET            ((ssize_t)(sizeof(list_t)))



list_t*     _ListInsertLast( list_t* node, list_t* insert );
// insert at end
// function returns a pointer to the first inserted node

list_t*     _ListInsertFirst( list_t* node, list_t* insert );
// insert at begin
// function returns a pointer to the first inserted node

list_t*     _ListInsertAfter( list_t* node, list_t* insert );
// insert after node
// function returns a pointer to the first inserted node

list_t*     _ListInsertBefore( list_t* node, list_t* insert );
// insert before node
// function returns a pointer to the first inserted node

list_t*     _ListExtract( list_t* node );
// extract a subtree from a tree
// function returns a pointer to the extracted node

list_t*     _ListExtractSublist( list_t* left, list_t* right );
// extract a sublist from a list
// function returns a pointer to the first inserted node

void        _ListNodeSetToNULL( list_t* node );
// set all pointers to NULL


// aliases
#define ListInsertLast(n,i)         ((void*)_ListInsertLast(((list_t*)(void*)(n)),(list_t*)(void*)(i)))
#define ListInsertFirst(n,i)        ((void*)_ListInsertFirst(((list_t*)(void*)(n)),(list_t*)(void*)(i)))
#define ListInsertAfter(n,i)        ((void*)_ListInsertAfter(((list_t*)(void*)(n)),(list_t*)(void*)(i)))
#define ListInsertBefore(n,i)       ((void*)_ListInsertBefore(((list_t*)(void*)(n)),(list_t*)(void*)(i)))
#define ListExtract(n)              ((void*)_ListExtract((list_t*)(void*)(n)))
#define ListExtractSublist(l,r)     ((void*)_ListExtractSublist((list_t*)(void*)(l),(list_t*)(void*)(r)))
#define ListNodeSetToNULL(n)        _ListNodeSetToNULL((list_t*)(void*)(n))

#define ListNext(n)                 ((void*)(((list_t*)(void*)(n))->next))
#define ListPrev(n)                 ((void*)(((list_t*)(void*)(n))->prev))


#endif //__LIST_H__