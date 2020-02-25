#include <stdio.h>

#include <core/var.h>
#include <core/string.h>

var_t* s_vol;
var_t* s_mouse;

int main( int argc, char** argv ) {
    VarInit();
    char scl[] = "warning";

    s_vol = VarCreate( "s_vol", VAR_FLT, "0.8", "0;100", "sound volume" );
    s_mouse = VarCreate( "s_mouse", VAR_FLT, "0.5", "0;1", "sound volume" );

    VarUpdate( s_vol, "22.412323" );

    printf( "name: %s; val: %f [%f;%f]\n", s_vol->name, s_vol->f, s_vol->fmin, s_vol->fmax );
    printf( "name: %s; val: %f [%f;%f]\n", s_mouse->name, s_mouse->f, s_mouse->fmin, s_mouse->fmax );
    //printf( "%d\n", s_vol->upd );

    VarShutdown();
    return 0;
}