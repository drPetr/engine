#include "zip.h"

#include <core/fs.h>
#include <core/memory.h>
#include <core/var.h>

var_t* inner_path_g;



typedef struct {
    bool_t      eread;      // reading error
    bool_t      eunksign;   // unknown signature
    bool_t      ecompres;   // compression is not supported
    bool_t      eversion;   // version needed to extract not support
    bool_t      eflags;     // flags no support
    bool_t      ezip64;     // zip64 not support
} zipErrorList_t;

typedef struct {
    const char* fileNmae;   // name of file to find
    bool_t      found;      // is file found
    ssize_t     offset;     // start of  data inner file
    size_t      dataSize;   // size of inner file
} findInZip_t;

/*
============
ZipFindEOCD

find the end of central directory, and return offset
if EOCD not found then return -1
============
*/
static ssize_t ZipFindEOCD( file_t* zip ) {
    ssize_t fsize;
    uint32_t sign;
    ssize_t offset;
    
    // get file size
    fsize = FileSize( zip );
    if( fsize < sizeof(zipEndOfCD_t) ) {
        return -1;
    }
    
    offset = fsize - sizeof(zipEndOfCD_t);
    // find the signature of the end of central directory
    do {
        FileSeek( zip, offset, F_SEEK_SET );
        if( sizeof(sign) != FileRead( &sign, sizeof(sign), zip ) ) {
            return -1;
        }
        
        // check the signature
        if( sign == ZIP_EOCD_SIGNATURE ) {
            // eocd found
            return offset;
        }
        offset--;
    } while( offset >= 0 );
    
    // cannot find the signature of the central directory 
    return -1;
}

/*
============
ZipReadEOCD

read EOCD structure and 
return values:
E_OK        if read
E_UNKFF     unknown file format
E_NORDF     no data to read
E_READ      read error
============
*/
static ecode_t ZipReadEOCD( file_t* zip, zipEndOfCD_t* eocd ) {
    ssize_t offset;
    ssize_t fsize;
    
    // find the EOCD
    offset = ZipFindEOCD( zip );
    if( offset == -1 ) {
        // unknown file format
        return E_NORDF;
    }
    // read the EOCD
    FileSeek( zip, offset, F_SEEK_SET );
    if( sizeof(zipEndOfCD_t) != FileRead( eocd, sizeof(zipEndOfCD_t), zip ) ) {
        return E_READ;
    }
    // check some values
    fsize = FileSize( zip );
    if( fsize < eocd->sizeOfCD || 
        eocd->offset > fsize - sizeof(zipEndOfCD_t) ||
        offset + eocd->comLen > fsize ) {
        return E_UNKFF;
    }
    
    // all okay
    return E_OK;
}

/*
============
ZipCheckProcCD

the function checks the zip file for data accuracy
============
*/
int ZipCheckProcCD( file_t* zip, const zipCDFileHeader_t* cdfh, 
            const char* fileName, const char* extra, const char* comment, void* userData ) {
    zipErrorList_t* e = (zipErrorList_t*)userData;
    zipLocalHeader_t lfh; // local file header
        
    // check version
    if( cdfh->extractVer > ZIP_SUPPORT_VERSION ) {
        e->eversion = btrue;
        return 0;
    }
    
    // check flags
    if( cdfh->flags != 0 ) {
        e->eflags = btrue;
        return 0;
    }
    
    // check for zip64
    if( cdfh->compressSize == 0xffffffff || cdfh->uncompressSize == 0xffffffff ) {
        e->ezip64 = btrue;
        return 0;
    }
    
    // check compression method
    if( cdfh->compress != ZIP_NO_COMPRESS ) {
        e->ecompres = btrue;
        return 0;
    }
        
    // read local file header
    FileSeek( zip, cdfh->offset, F_SEEK_SET );
    if( sizeof(lfh) != FileRead( &lfh, sizeof(lfh), zip ) ) {
        e->eread = btrue;
        return 0;
    }
    
    // check signature
    if( lfh.signature != ZIP_LFH_SIGNATURE ) {
        e->eunksign = btrue;
        return 0;
    }
    
    // check version
    if( lfh.extractVer > ZIP_SUPPORT_VERSION ) {
        e->eversion = btrue;
        return 0;
    }
    
    // check flags
    if( lfh.flags != 0 ) {
        e->eflags = btrue;
        return 0;
    }
    
    // check for zip64
    if( lfh.compressSize == 0xffffffff || lfh.uncompressSize == 0xffffffff ) {
        e->ezip64 = btrue;
        return 0;
    }
    
    // check compression method
    if( lfh.compress != ZIP_NO_COMPRESS ) {
        e->ecompres = btrue;
        return 0;
    }
    
    return 0;
}

/*
============
DosTimeToZipTime
============
*/
void DosTimeToZipTime( uint16_t date, uint16_t time, zipDateTime_t* tm ) {
    tm->day = date & 0x1f;
    tm->mon = (date >> 5) & 0xf;
    tm->year = ((date >> 9) & 0x7f) + 1980;
    tm->sec = (time & 0x1f) * 2;
    tm->min = (time >> 5) & 0x3f;
    tm->hour = (time >> 11) & 0x1f;
}

/*
============
ZipTimeToDosTime
============
*/
void ZipTimeToDosTime( const zipDateTime_t* tm, uint16_t* date, uint16_t* time ) {
    *date = (tm->day & 0x1f) | ((tm->mon & 0xf) << 5) | (((tm->year - 1980) & 0x7f) << 9);
    *time = ((tm->sec / 2) & 0x1f) | ((tm->min & 0x3f) << 5) | ((tm->hour & 0x1f) << 11);
}


/*
============
ZipCheckProc

the function checks the zip file for data accuracy
============
*/
int ZipCheckProc( file_t* zip, const zipLocalHeader_t* lfh, ssize_t offset,
            const char* fileName, const char* extra, void* userData ) {
    zipErrorList_t* e = (zipErrorList_t*)userData;
    
    // check version
    if( lfh->extractVer > ZIP_SUPPORT_VERSION ) {
        e->eversion = btrue;
        return 0;
    }
    
    // check flags
    if( lfh->flags != 0 ) {
        e->eflags = btrue;
        return 0;
    }
    
    // check for zip64
    if( lfh->compressSize == 0xffffffff || lfh->uncompressSize == 0xffffffff ) {
        e->ezip64 = btrue;
        return 0;
    }
    
    // check compression method
    if( lfh->compress != ZIP_NO_COMPRESS ) {
        e->ecompres = btrue;
        return 0;
    }
    
    return 0;
}

/*
============
ZipFindFile
============
*//*
static int ZipFindFile( file_t* zip, const zipLocalHeader_t* lfh, ssize_t offset,
            const char* fileName, const char* extra, void* userData ) {
    findInZip_t* findData = (findInZip_t*)userData;
    
    // check filename
    if( 0 == StrCmp( fileName, findData->fileNmae ) ) {
        findData->found = btrue;
        findData->offset = offset;
        findData->dataSize = (size_t)lfh->uncompressSize;
        return -1;
    }
    return 0;
}*/

/*
============
ZipProcessCD

process central directory
return values:
E_OK        if read
E_UNKFF     unknown file format
E_NORDF     no data to read
E_READ      read error
E_NOMEM     not enough memory
E_UNKCODE   unknown code
============
*/
ecode_t ZipProcessCD( file_t* zip, fnZipProcCD fnproc, void* userData ) {
    zipEndOfCD_t eocd;
    zipCDFileHeader_t cdfh;
    ecode_t e;
    ssize_t offset;
    ssize_t i;
    ssize_t total;
    char fileName[ZIP_FILENAME_MAX_LEN];
    char extraField[ZIP_EXTRA_FIELD_MAX_LEN];
    char fileComment[ZIP_FILE_COMMENT_MAX_LEN];
    int eproc;
    
    // read EOCD
    if( E_OK != (e = ZipReadEOCD( zip, &eocd )) ) {
        // central directory not found
        return e;
    }
    
    // seeking to the central directory file header
    offset = (ssize_t)eocd.offset;
    total = (ssize_t)eocd.totalNumInDisk;
    // for each CDFH
    for( i = 0; i < total; i++ ) {
        FileSeek( zip, offset, F_SEEK_SET );
        if( sizeof(cdfh) != FileRead( &cdfh, sizeof(cdfh), zip ) ) {
            // no data for read
            return E_READ;
        }
        // check signature
        if( cdfh.signature != ZIP_CDFH_SIGNATURE ) {
            // unknown file format
            return E_UNKFF;
        }
        
        // read filename
        if( cdfh.fnameLen >= sizeof(fileName) ) {
            return E_NOMEM;
        }
        if( cdfh.fnameLen != FileRead( fileName, cdfh.fnameLen, zip ) ) {
            // no data for read
            return E_READ;
        }
        
        // read extra field
        if( cdfh.extraLen >= sizeof(extraField) ) {
            return E_NOMEM;
        }
        if( cdfh.extraLen != FileRead( extraField, cdfh.extraLen, zip ) ) {
            // no data for read
            return E_READ;
        }
        
        // read file comment
        if( cdfh.fcomLen >= sizeof(fileComment) ) {
            return E_NOMEM;
        }
        if( cdfh.fcomLen != FileRead( fileComment, cdfh.fcomLen, zip ) ) {
            // no data for read
            return E_READ;
        }
        
        fileName[cdfh.fnameLen] = 0;
        extraField[cdfh.extraLen] = 0;
        fileComment[cdfh.fcomLen] = 0;
        
        // call callback function and process central directory
        eproc = fnproc( zip, &cdfh, fileName, extraField, fileComment, userData );
        if( eproc == -1 ) {
            return E_OK;
        } else if( eproc != 0 ) {
            return E_UNKCODE;
        } 
        
        // next offset
        offset += sizeof(cdfh) + cdfh.fnameLen + cdfh.extraLen + cdfh.fcomLen;
    }
    return E_OK;
}

/*
============
ZipProcess

process files in zip archive without use central directory
return values:
E_OK        if read
E_READ      read error
E_NOMEM     not enough memory
E_UNKCODE   unknown code
============
*/
ecode_t ZipProcess( file_t* zip, fnZipProc fnproc, void* userData ) {
    ecode_t eproc;
    ssize_t offset;
    ssize_t fsize;
    zipLocalHeader_t lfh;
    char fileName[ZIP_FILENAME_MAX_LEN];
    char extraField[ZIP_EXTRA_FIELD_MAX_LEN];
    
    // seeking to the central directory file header
    offset = 0;
    fsize = FileSize( zip );
    
    while( fsize > offset ) {
        FileSeek( zip, offset, F_SEEK_SET );
        if( sizeof(lfh) != FileRead( &lfh, sizeof(lfh), zip ) ) {
            // no data for read
            return E_READ;
        }
        
        // check signature
        if( lfh.signature != ZIP_LFH_SIGNATURE ) {
            // unknown file format
            return 0;
        }
        
        // read filename
        if( lfh.fnameLen >= sizeof(fileName) ) {
            return E_NOMEM;
        }
        if( lfh.fnameLen != FileRead( fileName, lfh.fnameLen, zip ) ) {
            // no data for read
            return E_READ;
        }
        
        // read extra field
        if( lfh.extraLen >= sizeof(extraField) ) {
            return E_NOMEM;
        }
        if( lfh.extraLen != FileRead( extraField, lfh.extraLen, zip ) ) {
            // no data for read
            return E_READ;
        }
        
        fileName[lfh.fnameLen] = 0;
        extraField[lfh.extraLen] = 0;
        
        offset += sizeof(lfh) + lfh.fnameLen + lfh.extraLen;
        // call callback function and process central directory
        eproc = fnproc( zip, &lfh, offset, fileName, extraField, userData );
        if( eproc == -1 ) {
            return E_OK;
        } else if( eproc != 0 ) {
            return E_UNKCODE;
        } 
        
        // next offset
        offset += lfh.uncompressSize;
    }
    
    return E_OK;
}

/*
============
ZipCheck
============
*/
ecode_t ZipCheck( file_t* zip ) {
    ecode_t e;
    zipErrorList_t elist;
    
    MemSet( &elist, 0, sizeof(elist) );
    
    // check zip file
    if( E_OK != (e = ZipProcess( zip, ZipCheckProc, &elist )) ) {
        return e;
    }
    if( elist.eread ) {
        return E_READ;
    }
    // may be more information
    if( elist.ecompres || elist.eversion || elist.eflags || elist.ezip64 ) {
        return E_UNKFF;
    }
    
    return e;
}





;
