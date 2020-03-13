#include <core/types.h>
#include <core/debug.h>

#include <stdio.h>

/*
============
PrintHexDump
============
*/
void PrintHexDump( void* fp, const void* data, size_t address, size_t len ) {
    FILE* f;
    char text[20];
    size_t i, j, indb;
    char* ptr;

    f = (FILE*)fp;

    fprintf( f, "address: 0x%.8Ix; len: %Iu\n", address, len );
    fprintf( f, "            " );
    for( i = 0; i < 16; i++ ) {
        fprintf( f, "%.2Ix ", i );
    }
    fprintf( f, "\n\n" );


    indb = address & 0xf;
    address &= ~0xf;
    text[16] = 0;
    ptr = (char*)data;


    if( indb ) {
        fprintf( f, "0x%.8Ix  ", address );
        for( i = 0; i < indb; i++ ) {
            fprintf( f, "   " );
            text[i] = ' ';
        }
    }


    for( j = 0; j < len ; i++, j++ ) {
        if( (i & 0xf) == 0 ) {
            fprintf( f, "0x%.8Ix  ", address );
        }

        fprintf( f, "%.2x ", (unsigned char)(*ptr) );
        text[i & 0xf] = (*ptr > 31 && *ptr < 128 ? *ptr : '.');

        if( (i & 0xf) == 15 ) {
            fprintf( f, " %s\n", text );
            address += 16;
        }

        ptr++;
    }

    if( i & 0xf ) {
        for( ; i & 0xf; i++ ) {
            fprintf( f, "   " );
            text[i & 0xf] = ' ';
        }
        fprintf( f, " %s\n", text );
    }
}
