#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "math_base.h"
#include "vector.h"
#include <core/types.h>


typedef struct {
    union {
        struct {
            vec2_t      a;
            vec2_t      b;
        };
        struct {
            float       m[4];
        };
    };
} mat2_t;

typedef struct {
    union {
        struct {
            vec3_t      a;
            vec3_t      b;
            vec3_t      c;
        };
        struct {
            float       m[9];
        };
    };
} mat3_t;

typedef struct {
    union {
        struct {
            vec4_t      a;
            vec4_t      b;
            vec4_t      c;
            vec4_t      d;
        };
        struct {
            float       m[16];
        };
    };
} mat4_t;






void        Mat2Set( mat2_t* m, const vec2_t* a, const vec2_t* b );
void        Mat3Set( mat3_t* m, const vec3_t* a, const vec3_t* b, const vec3_t* c );
void        Mat4Set( mat4_t* m, const vec4_t* a, const vec4_t* b, const vec4_t* c, const vec4_t* d );


void        Mat2Set4f( mat2_t* m, float ax, float ay, float bx, float by );
void        Mat3Set9f( mat3_t* m, float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz );
void        Mat4Set16f( mat4_t* m, float ax, float ay, float az, float aw, float bx, float by, float bz, float bw,
                        float cx, float cy, float cz, float cw, float dx, float dy, float dz, float dw );


void        Mat2Set4fv( mat2_t* m, const float* src );
void        Mat3Set9fv( mat3_t* m, const float* src );
void        Mat4Set16fv( mat4_t* m, const float* src );


void        Mat2Zero( mat2_t* m );
void        Mat3Zero( mat3_t* m );
void        Mat4Zero( mat4_t* m );


void        Mat2Ident( mat2_t* m );
void        Mat3Ident( mat3_t* m );
void        Mat4Ident( mat4_t* m );


void        Mat2Neg( mat2_t* m );
void        Mat3Neg( mat3_t* m );
void        Mat4Neg( mat4_t* m );


bool_t      Mat2Inv( mat2_t* m );
bool_t      Mat3Inv( mat3_t* m );
bool_t      Mat4Inv( mat4_t* m );


void        Mat2Scale( mat2_t* m, float s );
void        Mat3Scale( mat3_t* m, float s );
void        Mat4Scale( mat4_t* m, float s );


void        Mat2MulVec2( vec2_t* out, const mat2_t* m, const vec2_t* v );
void        Mat3MulVec3( vec3_t* out, const mat3_t* m, const vec3_t* v );
void        Mat4MulVec4( vec4_t* out, const mat4_t* m, const vec4_t* v );
void        Mat4MulVec3( vec3_t* out, const mat4_t* m, const vec3_t* v );


void        Vec2MulMat2( vec2_t* out, const vec2_t* v, const mat2_t* m );
void        Vec3MulMat3( vec3_t* out, const vec3_t* v, const mat3_t* m );
void        Vec4MulMat4( vec4_t* out, const vec4_t* v, const mat4_t* m );


void        Mat2Mul( mat2_t* out, const mat2_t* a, const mat2_t* b );
void        Mat3Mul( mat3_t* out, const mat3_t* a, const mat3_t* b );
void        Mat4Mul( mat4_t* out, const mat4_t* a, const mat4_t* b );


void        Mat2Add( mat2_t* out, const mat2_t* a, const mat2_t* b );
void        Mat3Add( mat3_t* out, const mat3_t* a, const mat3_t* b );
void        Mat4Add( mat4_t* out, const mat4_t* a, const mat4_t* b );


void        Mat2Sub( mat2_t* out, const mat2_t* a, const mat2_t* b );
void        Mat3Sub( mat3_t* out, const mat3_t* a, const mat3_t* b );
void        Mat4Sub( mat4_t* out, const mat4_t* a, const mat4_t* b );


bool_t      Mat2Cmp( const mat2_t* a, const mat2_t* b );
bool_t      Mat3Cmp( const mat3_t* a, const mat3_t* b );
bool_t      Mat4Cmp( const mat4_t* a, const mat4_t* b );


bool_t      Mat2CmpEps( const mat2_t* a, const mat2_t* b, float eps );
bool_t      Mat3CmpEps( const mat3_t* a, const mat3_t* b, float eps );
bool_t      Mat4CmpEps( const mat4_t* a, const mat4_t* b, float eps );


bool_t      Mat2IsDiag( const mat2_t* m );
bool_t      Mat3IsDiag( const mat3_t* m );
bool_t      Mat4IsDiag( const mat4_t* m );


bool_t      Mat2IsIdent( const mat2_t* m );
bool_t      Mat3IsIdent( const mat3_t* m );
bool_t      Mat4IsIdent( const mat4_t* m );


float       Mat2Det( const mat2_t* m );
float       Mat3Det( const mat3_t* m );
float       Mat4Det( const mat4_t* m );


void        Mat2Transp( mat2_t* m );
void        Mat3Transp( mat3_t* m );
void        Mat4Transp( mat4_t* m );


void        Mat2ToStr( char* out, const mat2_t* m, int prec );
void        Mat3ToStr( char* out, const mat3_t* m, int prec );
void        Mat4ToStr( char* out, const mat4_t* m, int prec );


void        Mat2ToMat3( mat3_t* out, const mat2_t* m );
void        Mat2ToMat4( mat4_t* out, const mat2_t* m );
void        Mat3ToMat2( mat2_t* out, const mat3_t* m );
void        Mat3ToMat4( mat4_t* out, const mat3_t* m );
void        Mat4ToMat2( mat2_t* out, const mat4_t* m );
void        Mat4ToMat3( mat3_t* out, const mat4_t* m );



#endif //__MATRIX_H__