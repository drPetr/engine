#ifndef __ECODE_H__
#define __ECODE_H__

// list of error codes

typedef enum {
    E_OK = 0,           // ok
    E_NUMRNG,           // number out of range
    E_NOTNUM,           // is not number
    E_NODATA,           // no data
    E_NOMEM,            // not enough memory
    E_BADF,             // bad file descriptor
    E_UNKFF,            // unknown file format
    E_NORDF,            // no data to read from file
    E_READ,             // read error (from file, from memory...)
    E_UNKCODE,          // unknown code (use in callback functions that return specific values)
    E_NOTFOUND          // value/file not found
} ecode_t;

#define OK      E_OK


#endif //__ECODE_H__