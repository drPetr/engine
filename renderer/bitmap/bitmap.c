#include "bitmap.h"

#include <assert.h>
#include <string.h>


/*
frgba_t clBlack  = { 0.0f,   0.0f,   0.0f,   1.0f };
frgba_t clRed    = { 1.0f,   0.0f,   0.0f,   1.0f };
frgba_t clGreen  = { 0.0f,   1.0f,   0.0f,   1.0f };
frgba_t clBlue   = { 0.0f,   0.0f,   1.0f,   1.0f };
frgba_t clYellow = { 1.0f,   1.0f,   0.0f,   1.0f };
frgba_t clMagenta= { 1.0f,   0.0f,   1.0f,   1.0f };
frgba_t clCyan   = { 0.0f,   1.0f,   1.0f,   1.0f };
frgba_t clWhite  = { 1.0f,   1.0f,   1.0f,   1.0f };
frgba_t clLtGrey = { 0.75f,  0.75f,  0.75f,  1.0f };
frgba_t clMdGrey = { 0.5f,   0.5f,   0.5f,   1.0f };
frgba_t clDkGrey = { 0.25f,  0.25f,  0.25f,  1.0f };
*/


#define BITMAP_1BIT         1
#define BITMAP_4BIT         4
#define BITMAP_8BIT         8
#define BITMAP_16BIT        16
#define BITMAP_24BIT        24
#define BITMAP_32BIT        32

/*
================
BitmapCheckBitPP

return:
1: if bpp is correct
0: if bpp is not correct
================
*/
static int BitmapCheckBitPP( dword_t bpp ) {
    return !!(bpp == BITMAP_8BIT || bpp == BITMAP_16BIT ||
        bpp == BITMAP_24BIT || bpp == BITMAP_32BIT);
}

/*
================
BitmapCheckPxfmt
================
*/
static int BitmapCheckPxfmt( dword_t pxfmt ) {
    return !!(pxfmt == IMAGE_FMT_BYTE1 || pxfmt == IMAGE_FMT_BYTE2 ||
        pxfmt == IMAGE_FMT_BYTE3 || pxfmt == IMAGE_FMT_BYTE4 ||
        pxfmt == IMAGE_FMT_FLOAT3 || pxfmt == IMAGE_FMT_FLOAT4);
}

/*
================
BitmapCheckPxfmtForSave
================
*/
static int BitmapCheckPxfmtForSave( dword_t pxfmt ) {
    return !!(pxfmt == IMAGE_FMT_BYTE1 || pxfmt == IMAGE_FMT_BYTE2 ||
        pxfmt == IMAGE_FMT_BYTE3 || pxfmt == IMAGE_FMT_BYTE4 );
}

/*
================
BitmapPaletteSizeFromPxfmt

return:
size of palette for pxfmt.
================
*/
static int BitmapPaletteSizeFromPxfmt( dword_t pxfmt ) {
    switch( pxfmt ) {
        case IMAGE_FMT_BYTE1:
            return 256;
    }
    return 0;
}

/*
================
BitmapBitPerPixelToPxfmt
================
*/
static int BitmapBitPerPixelToPxfmt( word_t bpp ) {
    switch( bpp ) {
        case BITMAP_8BIT:
            return IMAGE_FMT_BYTE1;
        case BITMAP_16BIT:
            return IMAGE_FMT_BYTE2;
        case BITMAP_24BIT:
            return IMAGE_FMT_BYTE3;
        case BITMAP_32BIT:
            return IMAGE_FMT_BYTE4;
        default:
            assert( 0 );
    }
}

/*
================
BitmapPxfmtToBitPerPixel
================
*/
static int BitmapPxfmtToBitPerPixel( word_t pxfmt ) {
    switch( pxfmt ) {
        case IMAGE_FMT_BYTE1:       // 8
            return BITMAP_8BIT;
        case IMAGE_FMT_BYTE2:       // 16
            return BITMAP_16BIT;
        case IMAGE_FMT_BYTE3:       // 24
            return BITMAP_24BIT;
        case IMAGE_FMT_BYTE4:       // 32
            return BITMAP_32BIT;
        case IMAGE_FMT_FLOAT3:      // 96
            return sizeof(float) * 8 * 3;
        case IMAGE_FMT_FLOAT4:      // 128
            return sizeof(float) * 8 * 4;
        default:
            assert( 0 );
    }
    return 0;
}

/*
================
BitmapPxfmtToBytePerPixel
================
*/
static int BitmapPxfmtToBytePerPixel( word_t pxfmt ) {
    return BitmapPxfmtToBitPerPixel(pxfmt) >> 3;
}

/*
================
BitmapLoadFromFile
================
*/
int BitmapLoadFromFile( FILE* f, bitmap_t* bmp ) {
    assert(f);
    assert(bmp);
    
    int i;
    bitmapFileHeader_t header;
    bitmapInfoHeader_t info;
    bitmapRgbQuad_t quad;
    dword_t stride;
    int dataPadding;
    int rowSize;
    byte_t* memptr;
    rgb_t* rgbptr;
    
    // Check for correct data
    memset( bmp, 0, sizeof(bitmap_t) );
    fseek( f, 0, SEEK_SET );
    if( fread( &header, sizeof(header), 1, f ) != 1 ) {
        fprintf( stderr, "error: unable to read bitmap file header\n" );
        return -1;
    }
    if( fread( &info, sizeof(info), 1, f ) != 1 ) {
        fprintf( stderr, "error: unable to read bitmap info header\n" );
        return -1;
    }
    if( header.signature != BITMAP_SIGNATURE ) {
        fprintf( stderr, "error: invalid bitmap file format\n" );
        return -1;
    }
    if( info.size != sizeof(info) ) {
        fprintf( stderr, "error: bitmap file format is not supported\n" );
        return -1;
    }
    if( !BitmapCheckBitPP(info.bitCount) ) {
        fprintf( stderr, "error: bitmap pixel format is not supported\n" );
        return -1;
    }
    if( info.width <= 0 || info.height <= 0 ) {
        fprintf( stderr, "error: incorrect bitmap image size\n" );
        return -1;
    }
    if( info.compression != BITMAP_RGB ) {
        fprintf( stderr, "error: bitmap compression is not supported\n" );
        return -1;
    }
    
    // Read bitmap palette
    if( !info.colorUsed ) {
        info.colorUsed = BitmapPaletteSizeFromPxfmt( info.bitCount );
    }
    if( info.colorUsed ) {
        memptr = (byte_t*)malloc(info.colorUsed * sizeof(rgb_t));
        bmp->paletteSize = info.colorUsed;
        rgbptr = (rgb_t*)memptr;
        bmp->palette = rgbptr;
        for( i = 0; i < info.colorUsed; i++ ) {
            if( fread( &quad, sizeof(quad), 1, f ) != 1 ) {
                free(memptr);
                fprintf( stderr, "error: unable to read bitmap palette\n" );
                return -1;
            }
            rgbptr->blue = quad.blue;
            rgbptr->green = quad.green;
            rgbptr->red = quad.red;
            rgbptr++;
        }
    }
    
    // Read bitmap data
    bmp->dataSize = ((info.width * info.height * info.bitCount) >> 3);
    memptr = malloc( bmp->dataSize );
    bmp->data = memptr;
    stride = ((info.bitCount * info.width + 31) & ~31) >> 3;
    rowSize = (((info.bitCount * info.width + 7) & ~7) >> 3);
    dataPadding = stride - rowSize;
    fseek( f, header.offset, SEEK_SET );
    for( i = 0; i < info.height; i++ ) {
        // set pointer for copying data
        memptr = bmp->data + rowSize * (info.height - i - 1);
        if( fread( memptr, rowSize, 1, f ) != 1 ) {
            if( bmp->palette ) {
                free( bmp->palette );
            }
            free( bmp->data );
            fprintf( stderr, "error: unable to read bitmap data\n" );
            return -1;
        }
        if( dataPadding ) {
            fseek( f, dataPadding, SEEK_CUR );
        }
    }
    
    // Set other data
    bmp->pxfmt = BitmapBitPerPixelToPxfmt( info.bitCount );
    bmp->bpp = ((info.bitCount + 7) & ~7) >> 3;
    bmp->stride = rowSize;
    bmp->width = info.width;
    bmp->height = info.height;
    
    return 0;
}

/*
================
BitmapSaveToFile
================
*/
int BitmapSaveToFile( FILE* f, bitmap_t* bmp ) {
    assert(f);
    assert(bmp);
    
    int i;
    bitmapFileHeader_t header;
    bitmapInfoHeader_t info;
    bitmapRgbQuad_t quad;
    int dataPadding;
    int rowSize;
    dword_t stride;
    dword_t offset;
    word_t bitpp;
    rgb_t* rgbptr;
    byte_t* data;
    int zero = 0;
    
    // Check for correct data
    bitpp = BitmapPxfmtToBitPerPixel(bmp->pxfmt);
    if( !BitmapCheckPxfmt(bmp->pxfmt) ) {
        fprintf( stderr, "error: pixel format is not supported\n" );
        return -1;
    }
    if( !BitmapCheckPxfmtForSave(bmp->pxfmt) ) {
        fprintf( stderr, "error: pixel format is not supported to save. use other pixel format\n" );
        return -1;
    }
    if( bitpp != (bmp->bpp << 3) ) {
        fprintf( stderr, "error: bit per pixel does not match\n" );
        return -1;
    }
    if( !BitmapCheckBitPP(bmp->bpp << 3) ) {
        fprintf( stderr, "error: incorrect bits per pixel\n" );
        return -1;
    }
    if( bmp->width <= 0 || bmp->height <= 0 ) {
        fprintf( stderr, "error: incorrect bitmap image size\n" );
        return -1;
    }
    if( bmp->data == NULL ) {
        fprintf( stderr, "error: data is empty\n" );
        return -1;
    }
    if( bmp->paletteSize != 0 && bmp->palette == NULL ) {
        fprintf( stderr, "error: palette is empty\n" );
        return -1;
    }
    
    offset = sizeof(header) + sizeof(info) + sizeof(quad) * bmp->paletteSize;
    stride = ((bitpp * bmp->width + 31) & ~31) >> 3;
    rowSize = ((bmp->width * bitpp + 7) & ~7) >> 3;
    dataPadding = stride - rowSize;
    
    // Set bitmap file header
    header.signature = BITMAP_SIGNATURE;
    header.size = offset + stride * bmp->height;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = offset;
    
    // Set bitmap info header
    info.size = sizeof(info);
    info.width = bmp->width;
    info.height = bmp->height;
    info.planes = 1;
    info.bitCount = bitpp;
    info.compression = BITMAP_RGB;
    info.imageSize = stride * bmp->height;
    info.xPxPerMeter = 0;
    info.yPxPerMeter = 0;
    info.colorUsed = bmp->paletteSize;
    info.colorImportant = 0;
    
    // Write data to file
    if( fwrite( &header, sizeof(header), 1, f ) != 1 ) {
        fprintf( stderr, "error: unable to write bitmap file header\n" );
        return -1;
    }
    if( fwrite( &info, sizeof(info), 1, f ) != 1 ) {
        fprintf( stderr, "error: unable to write bitmap info header\n" );
        return -1;
    }
    
    // Write palette
    if( bmp->paletteSize != 0 ) {
        rgbptr =  bmp->palette;
        for( i = 0; i < bmp->paletteSize; i++ ) {
            quad.blue = rgbptr->blue;
            quad.green = rgbptr->green;
            quad.red = rgbptr->red;
            quad.reserved = 0;
            if( fwrite( &quad, sizeof(quad), 1, f ) != 1 ) {
                fprintf( stderr, "error: unable to write bitmap palette\n" );
                return -1;
            }
            rgbptr++;
        }
    }
    
    // Write bitmap data
    for( i = 0; i < bmp->height; i++ ) {
        data = bmp->data + bmp->stride * (bmp->height - i - 1);
        if( fwrite( data, rowSize, 1, f ) != 1 ) {
            fprintf( stderr, "error: unable to write bitmap data\n" );
            return -1;
        }
        fwrite( &zero, dataPadding, 1, f );
    }
    
    return 0;
}

/*
================
BitmapCreate
================
*/
void BitmapCreate( bitmap_t* bmp, int width, int height, word_t pxfmt ) {
    assert( bmp );
    assert( width > 0 );
    assert( height > 0 );
    assert( BitmapCheckPxfmt(pxfmt) );
    
    int paletteSize;
    int rowSize;
    int bitpp;
    
    // Check bit per pixels
    memset( bmp, 0, sizeof(bitmap_t) );
    bitpp = BitmapPxfmtToBitPerPixel(pxfmt);
    paletteSize = BitmapPaletteSizeFromPxfmt(pxfmt);
    rowSize = ((width * bitpp + 7) & ~7) >> 3;
    
    // Set all data
    if( paletteSize != 0 ) {
        bmp->palette = (rgb_t*)malloc( paletteSize * sizeof(rgb_t) );
    }
    bmp->dataSize = rowSize * height;
    bmp->data = (byte_t*)malloc(bmp->dataSize);
    bmp->paletteSize = paletteSize;
    bmp->stride = rowSize;
    bmp->pxfmt = pxfmt;
    bmp->bpp = bitpp >> 3;
    bmp->width = width;
    bmp->height = height;
}

/*
================
BitmapFree
================
*/
void BitmapFree( bitmap_t* bmp ) {
    assert(bmp);
    
    // Free palette and bitmap data
    if( bmp->palette ) {
        free( bmp->palette );
        bmp->palette = NULL;
        bmp->paletteSize = 0;
    }
    if( bmp->data ) {
        free( bmp->data );
        bmp->data = 0;
        bmp->dataSize = 0;
    }
}

/*
================
BitmapResize
================
*/
void BitmapResize( bitmap_t* bmp, int width, int height ) {
    assert(bmp);
    assert(width > 0);
    assert(height > 0);
    assert(bmp->data);
    assert(bmp->width > 0);
    assert(bmp->height > 0);
    
    int i;
    int minHeight;
    dword_t newStride;
    dword_t minStride;
    dword_t dataSize;
    byte_t* data;
    byte_t* cpyfrom;
    byte_t* cpyto;
    
    // Copy bitmap data to new bitmap buffer
    minHeight = height < bmp->height ? height : bmp->height;
    newStride = width * bmp->bpp;
    minStride = newStride < bmp->stride ? newStride : bmp->stride;
    dataSize = newStride * height;
    data = (byte_t*)malloc(dataSize);
    for( i = 0; i < minHeight; i++ ) {
        cpyfrom = bmp->data + bmp->stride * i;
        cpyto = data + newStride * i;
        memcpy( cpyto, cpyfrom, minStride );
    }
    free( bmp->data );
    bmp->data = data;
    bmp->dataSize = dataSize;
    bmp->stride = newStride;
    bmp->width = width;
    bmp->height = height;
}

/*
================
BitmapArea
================
*/
void BitmapArea( bitmap_t* dst, bitmap_t* src, int left, int top, int width, int height ) {
    assert(dst);
    assert(src);
    assert(width > 0);
    assert(height > 0);
    assert(left >= 0);
    assert(top >= 0);
    assert(left + width <= src->width);
    assert(top + height <= src->height);
    assert(src->data);
    
    dst->ref = src;
    dst->palette = src->palette;
    dst->data = src->data + src->bpp * left + src->bpp * src->width * top;
    dst->paletteSize = src->paletteSize;
    dst->dataSize = src->dataSize;
    dst->stride = src->stride;
    dst->pxfmt = src->pxfmt;
    dst->bpp = src->bpp;
    dst->left = left;
    dst->top = top;
    dst->width = width;
    dst->height = height;
}

/*
================
BitmapCopy
================
*/
int BitmapCopy( bitmap_t* dst, bitmap_t* src, word_t pxfmt ) {
    assert(dst);
    assert(src);
    assert(src->data);
    assert(BitmapCheckPxfmt(src->pxfmt) || pxfmt == 0);
    
    int i, j;
    bitmap_t bmp;
    int deqs = dst == src;  // dst equal src;
    if( deqs ) {
        dst = &bmp;
    }
    
    // Copy if not convert
    pxfmt = pxfmt == 0 ? src->pxfmt : pxfmt;
    if( src->pxfmt == pxfmt ) {
        if( deqs ) {
            return 0;
        }
        BitmapCreate( dst, src->width, src->height, pxfmt );
        memcpy( dst->data, src->data, src->dataSize );
        return 0;
    }
    
    // Convertations
    if( src->pxfmt == IMAGE_FMT_BYTE3 && pxfmt == IMAGE_FMT_FLOAT4 ) {
        BitmapCreate( dst, src->width, src->height, pxfmt );
        rgb_t* s;
        frgba_t* d = (frgba_t*)dst->data;
        for( j = 0; j < src->height; j++ ) {
            s = (rgb_t*)(src->data + src->stride * j);
            for( i = 0; i < src->width; i++ ) {
                d->alpha = 1.0f;
                d->blue = (float)s->blue / 255.0f;
                d->green = (float)s->green / 255.0f;
                d->red = (float)s->red / 255.0f;
                d++;
                s++;
            }
        }
    } else if( src->pxfmt == IMAGE_FMT_FLOAT4 && pxfmt == IMAGE_FMT_BYTE3 ) {
        BitmapCreate( dst, src->width, src->height, pxfmt );
        frgba_t* s;
        rgb_t* d = (rgb_t*)dst->data;
        for( j = 0; j < src->height; j++ ) {
            s = (frgba_t*)(src->data + src->stride * j);
            for( i = 0; i < src->width; i++ ) {
                //assert( s <= (src->data + src->dataSize) );
                d->blue = (byte_t)(AlphaBlend(1.0f, s->blue, s->alpha) * 255.0f);
                d->green = (byte_t)(AlphaBlend(1.0f, s->green, s->alpha) * 255.0f);
                d->red = (byte_t)(AlphaBlend(1.0f, s->red, s->alpha) * 255.0f);
                d++;
                s++;
            }
        }
    } else {
        fprintf( stderr, "error: convertation is not supported\n" );
        return -1;
    }
    
    // If destination == source 
    if( deqs ) {
        BitmapFree( src );
        memcpy( src, dst, sizeof(bitmap_t) );
    }
    
    return 0;
}

