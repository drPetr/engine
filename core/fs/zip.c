#include <stdio.h>

#include <core/alloc.h>

#include "zip.h"
#include <string.h>

#define zip_st_allocate(mem)    GetZipFileStructAllocator()->allocate(mem)
#define zip_st_deallocate(ptr)  GetZipFileStructAllocator()->deallocate(ptr)

/*
============
fsize

return size of file
============
*/
static ssize_t fsize( FILE* f ) {
    int64_t pos;
    int64_t size;
    
    fgetpos( f, &pos );
    fseek( f, 0, SEEK_END );
    fgetpos( f, &size );
    fsetpos( f, &pos );
    
    return size > 0 ? (ssize_t)size : -1;
}

/*
============
ZipFindEOCD

find the end of central directory, and return offset
if EOCD not found then return -1
============
*/
static ssize_t ZipFindEOCD( FILE* zip ) {
    zipLocalHeader_t lfh;
    
    ssize_t fileSize;
    uint32_t sign;
    ssize_t offset;
    ssize_t offsetTo;
    
    // get file size
    
    
    
    fileSize = fsize( zip );
    if( fileSize < sizeof(zipLocalHeader_t) ) {
        return -1;
    }
    
    fseek( zip, SEEK_SET, 0 );
    offsetTo = 0;
    // check minimum size
    if( sizeof(lfh) != fread( &lfh, 1, sizeof(lfh), zip ) ) {
        // cannot read file
        return -1;
    }
    // check signature
    if( lfh.signature != ZIP_LFH_SIGNATURE ) {
        return -1;
    }
    
    // calculate min offset
    do {
        offsetTo += sizeof(lfh) + lfh.fnameLen + lfh.extraLen + lfh.compressSize;
        if( sizeof(lfh) != fread( &lfh, 1, sizeof(lfh), zip ) ) {
            // cannot read file
            return -1;
        }
    } while( lfh.signature == ZIP_LFH_SIGNATURE );
    
    
    offset = fileSize - sizeof(zipEndOfCD_t);
    // find the signature of the end of central directory
    do {
        fseek( zip, offset, SEEK_SET );
        if( sizeof(sign) != fread( &sign, 1, sizeof(sign), zip ) ) {
            // cannot read file
            return -1;
        }
        
        // check the signature
        if( sign == ZIP_EOCD_SIGNATURE ) {
            // eocd found
            return offset;
        }
        offset--;
    } while( offset >= offsetTo );
    
    // cannot find the signature of the central directory 
    return -1;
}

/*
============
ZipReadEOCD

read EOCD structure and 
return values:
0       if read
-1      if was an error
============
*/
static int ZipReadEOCD( FILE* zip, zipEndOfCD_t* eocd ) {
    ssize_t offset;
    ssize_t fileSize;
    
    // find the EOCD
    offset = ZipFindEOCD( zip );
    if( offset == -1 ) {
        // unknown file format
        return -1;
    }
    // read the EOCD
    fseek( zip, offset, SEEK_SET );
    if( sizeof(zipEndOfCD_t) != fread( eocd, 1, sizeof(zipEndOfCD_t), zip ) ) {
        return -1;
    }
    // check some values
    fileSize = fsize( zip );
    if( fileSize < eocd->sizeOfCD || 
            eocd->offset > fileSize - sizeof(zipEndOfCD_t) ||
            offset + eocd->comLen > fileSize ) {
        return -1;
    }
    
    // all okay
    return 0;
}



/*
============
ZipOpen
============
*/
zipf_t* ZipOpen( const char* zipFile, uint32_t om ) {
    zipf_t* zip;
    
    zip = (zipf_t*)zip_st_allocate(sizeof(zipf_t));
    
    if( NULL == (zip->file = fopen( zipFile, "rb" )) ) {
        zip_st_deallocate( zip );
        return NULL;
    }
    
    // read eocd
    if( 0 != ZipReadEOCD( zip->file, &(zip->eocd) ) ) {
        zip_st_deallocate( zip );
        return NULL;
    }
    
    zip->loaded = btrue;
    
    return zip;
    
}

/*
============
ZipClose
============
*/
int ZipClose( zipf_t* zip ) {
    if( zip->file ) {
        fclose( zip->file );
    }
    zip_st_deallocate( zip );
    
    return 0;
}

/*
============
ZipFirstFile
============
*/
int ZipFirstFile( zipf_t* zip ) {
    zip->curOffset = zip->eocd.offset;
    zip->stop = btrue;
    
    // seek to first CD file header
    fseek( zip->file, (long int)zip->curOffset, SEEK_SET );
    // read inner file structure
    if( sizeof(zipCDFileHeader_t) != fread( &(zip->cdfh), 1, sizeof(zipCDFileHeader_t), zip->file ) ) {
        return -1;
    }
    
    // check signature
    if( zip->cdfh.signature != ZIP_CDFH_SIGNATURE ) {
        return -1;
    }
    
    zip->stop = bfalse;
    return 0;
}

/*
============
ZipNextFile
============
*/
int ZipNextFile( zipf_t* zip ) {    
    if( zip->stop ) {
        return -1;
    }
    
    // next offset
    zip->curOffset += sizeof(zipCDFileHeader_t) + zip->cdfh.fnameLen + zip->cdfh.extraLen + zip->cdfh.fcomLen;
    
    // seek to next CD file header
    fseek( zip->file, (long int)zip->curOffset, SEEK_SET );
    // read inner file structure
    if( sizeof(zipCDFileHeader_t) != fread( &(zip->cdfh), 1, sizeof(zipCDFileHeader_t), zip->file ) ) {
        zip->stop = btrue;
        return -1;
    }
    
    // check signature
    if( zip->cdfh.signature != ZIP_CDFH_SIGNATURE ) {
        zip->stop = btrue;
        return -1;
    }
    
    return 0;
}

/*
============
ZipGetCurrentFileName
============
*/
ssize_t ZipGetCurrentFileName( zipf_t* zip, char* name, size_t maxSize ) {
    ssize_t size;
    
    if( zip->stop ) {
        return -1;
    }
    
    // read current file name
    size = maxSize > zip->cdfh.fnameLen ? zip->cdfh.fnameLen : (ssize_t)maxSize - 1;
    fseek( zip->file, zip->curOffset + sizeof(zipCDFileHeader_t), SEEK_SET );
    if( size != fread( name, 1, size, zip->file ) ) {
        return -1;
    }
    name[size] = 0;
    
    return size;
}

/*
============
ZipTotalFiles
============
*/
ssize_t ZipTotalFiles( zipf_t* zip ) {
    return zip->loaded ? (ssize_t)zip->eocd.totalNumInDisk : -1;
}