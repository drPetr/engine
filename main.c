#include <stdio.h>

#include <core/var.h>
#include <core/string.h>

int main( int argc, char** argv ) {
    const char s[] = "  -2147483648    asdf";
    const char* end = NULL;
    int i = 0;
    
    printf( "%d\n", StrToInt( s, &end, NULL ) );
    printf( "%s\n", end );
    printf( "%d\n", i );

    return 0;
}