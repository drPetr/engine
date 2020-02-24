#ifndef __ECODE_H__
#define __ECODE_H__

// list of error codes

typedef enum {
    E_OK = 0,           // ok
    E_NUMRNG,           // number out of range
    E_NOTNUM,           // is not number
    E_NODATA,           // no data
} ecode_t;


#endif //__ECODE_H__