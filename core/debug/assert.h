#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdlib.h>
#include <stdio.h>

#ifdef NO_ASSERT
    #define assert(e)
    #define asserta(e,a,...)
#else
    #define assert(e)                                           \
        if(!(e)){                                               \
            fprintf(stderr, "assertion failed: %s\n", #e);      \
            fprintf(stderr, "file: %s\n", __FILE__);            \
            fprintf(stderr, "line: %d\n", __LINE__);            \
            abort();                                            \
        } else (void)(0)
    #define asserta(e,a,...)     do {                           \
        if(!(e)){                                               \
            fprintf(stderr, "assertion failed: %s\n", #e);      \
            fprintf(stderr, "file: %s\n", __FILE__);            \
            fprintf(stderr, "line: %d\n", __LINE__);            \
            fprintf(stderr, a, ##__VA_ARGS__);                  \
            abort();                                            \
        }                                                       \
    } while(0)
#endif

#endif // __ASSERT_H__