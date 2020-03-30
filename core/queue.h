#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <core/types.h>

// queue
typedef struct queue_t {
    struct queue_t* next;
    struct queue_t* prev;
    uint8_t         data[0];
} queue_t;

#define QUEUE_DATA_OFFSET       ((ssize_t)(sizeof(queue_t)))



queue_t*    _QueuePush( queue_t* node, queue_t* insert );
queue_t*    _QueuePop( queue_t* node );

#define     QueuePush(n,i)      ((void*)_QueuePush((queue_t*)(void*)(n),(queue_t*)(void*)(i)))
#define     QueuePop(n)         ((void*)_QueuePop((queue_t*)(void*)(n)))



#endif //__QUEUE_H__