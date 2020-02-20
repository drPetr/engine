#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "math_base.h"
#include <core/types.h>

typedef struct {
    union {
        struct {
            float           x;
            float           y;
        };
        struct {
            float           m[2];   // memory access
        };
    };
} vec2_t;

typedef struct {
    union {
        struct {
            union {
                struct {
                    float   x;
                    float   y;
                };
                vec2_t      vec2;   // vector 2 value
            };
            float           z;      //
        };
        struct {
            float           m[3];   // memory access
        };
    };
} vec3_t;

typedef struct {
    union {
        struct {
            union {
                struct {
                    float   x;
                    float   y;
                    float   z;
                };
                vec3_t      vec3;   // vector 3 value
            };
            float           w;      // 
        };
        struct {
            float           m[4];   // memory access
        };
    };
} vec4_t;






void        Vec2Set( vec2_t* v, float x, float y );
void        Vec3Set( vec3_t* v, float x, float y, float z );
void        Vec4Set( vec4_t* v, float x, float y, float z, float w );
// set vector values

void        Vec2Cpy( vec2_t* out, const vec2_t* v );
void        Vec3Cpy( vec3_t* out, const vec3_t* v );
void        Vec4Cpy( vec4_t* out, const vec4_t* v );
// copy values from v to out

void        Vec2Zero( vec2_t* v );
void        Vec3Zero( vec3_t* v );
void        Vec4Zero( vec4_t* v );
// set values to zero

void        Vec2Neg( vec2_t* v );
void        Vec3Neg( vec3_t* v );
void        Vec4Neg( vec4_t* v );
// set values to negative

void        Vec2Inv( vec2_t* v );
void        Vec3Inv( vec3_t* v );
void        Vec4Inv( vec4_t* v );
// set inverse values

void        Vec2Scale( vec2_t* v, const vec2_t* s );
void        Vec3Scale( vec3_t* v, const vec3_t* s );
void        Vec4Scale( vec4_t* v, const vec4_t* s );
// multiply the values v by each of s

void        Vec2Scale1f( vec2_t* v, float f );
void        Vec3Scale1f( vec3_t* v, float f );
void        Vec4Scale1f( vec4_t* v, float f );
// multiply the values v by f

void        Vec2Add( vec2_t* out, const vec2_t* a, const vec2_t* b );
void        Vec3Add( vec3_t* out, const vec3_t* a, const vec3_t* b );
void        Vec4Add( vec4_t* out, const vec4_t* a, const vec4_t* b );
// add two vectors a and b, and write the result out

void        Vec2Sub( vec2_t* out, const vec2_t* a, const vec2_t* b );
void        Vec3Sub( vec3_t* out, const vec3_t* a, const vec3_t* b );
void        Vec4Sub( vec4_t* out, const vec4_t* a, const vec4_t* b );
// subtract two vectors a and b, and write the result out

void        Vec3Cross( vec3_t* out, const vec3_t* a, const vec3_t* b );
// vector product of vectors a and b, and write the result out

bool_t      Vec2Cmp( const vec2_t* a, const vec2_t* b );
bool_t      Vec3Cmp( const vec3_t* a, const vec3_t* b );
bool_t      Vec4Cmp( const vec4_t* a, const vec4_t* b );
// compare two vectors and return btrue if vectors is equal, or bfalse in other case

bool_t      Vec2CmpEps( const vec2_t* a, const vec2_t* b, float eps );
bool_t      Vec3CmpEps( const vec3_t* a, const vec3_t* b, float eps );
bool_t      Vec4CmpEps( const vec4_t* a, const vec4_t* b, float eps );
// compare two vectors with using epsilon and return btrue if vectors is equal, or bfalse in other case

float       Vec2Len( const vec2_t* a, const vec2_t* b );
float       Vec3Len( const vec3_t* a, const vec3_t* b );
float       Vec4Len( const vec4_t* a, const vec4_t* b );
// return the length of the vector

float       Vec2SqrLen( const vec2_t* a, const vec2_t* b );
float       Vec3SqrLen( const vec3_t* a, const vec3_t* b );
float       Vec4SqrLen( const vec4_t* a, const vec4_t* b );
// return the squared length of the vector

float       Vec2Norm( vec2_t* v );
float       Vec3Norm( vec3_t* v );
float       Vec4Norm( vec4_t* v );
// normalize vector and return the length of the vector

float       Vec2Dot( const vec2_t* a, const vec2_t* b );
float       Vec3Dot( const vec3_t* a, const vec3_t* b );
float       Vec4Dot( const vec4_t* a, const vec4_t* b );
// return the dot product between two vectors

float       Vec2Cos( const vec2_t* a, const vec2_t* b );
float       Vec3Cos( const vec3_t* a, const vec3_t* b );
// return the cosine between two vectors

float       Vec2Angle( const vec2_t* a, const vec2_t* b );
float       Vec3Angle( const vec3_t* a, const vec3_t* b );
// return the angle in radians between two vectors

void        Vec2Clamp( vec2_t* v, const vec2_t* min, const vec2_t* max );
void        Vec3Clamp( vec3_t* v, const vec3_t* min, const vec3_t* max );
void        Vec4Clamp( vec4_t* v, const vec4_t* min, const vec4_t* max );
// the clamped value of v between the min and max. clamped value is written in v

void        Vec2Lerp( vec2_t* out, const vec2_t* a, const vec2_t* b, float s );
void        Vec3Lerp( vec3_t* out, const vec3_t* a, const vec3_t* b, float s );
void        Vec4Lerp( vec4_t* out, const vec4_t* a, const vec4_t* b, float s );
// linear interpolation between two vectors a and b with the scale factor

void        Vec2ToStr( char* out, const vec2_t* v, int prec );
void        Vec3ToStr( char* out, const vec3_t* v, int prec );
void        Vec4ToStr( char* out, const vec4_t* v, int prec );
// writing a vector into a string

void        Vec2ToVec3( vec3_t* out, const vec2_t* v ); 
void        Vec2ToVec4( vec4_t* out, const vec2_t* v );
void        Vec3ToVec2( vec2_t* out, const vec3_t* v );
void        Vec3ToVec4( vec4_t* out, const vec3_t* v );
void        Vec4ToVec2( vec2_t* out, const vec4_t* v );
void        Vec4ToVec3( vec3_t* out, const vec4_t* v );
// conversion vectors



#endif //__VECTOR_H__