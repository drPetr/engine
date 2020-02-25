#ifndef __MATH_BASE_H__
#define __MATH_BASE_H__



const float  PI = 3.14159265358979323846;           // pi
//const float  TWO_PI = PI * 2.0;                     // pi * 2
//const float  HALF_PI = PI / 2.0;                    // pi / 2
//const float  INV_PI = 1.0 / PI;                     // 1 / pi
const float  E = 2.71828182845904523536;            // e
const float  SQRT_TWO = 1.41421356237309504880;     // sqrt( 2 )
const float  SQRT_THREE = 1.73205080756887729352;   // sqrt( 3 )
const float  SQRT_HALF = 0.70710678118654752440;    // sqrt( 1 / 2 )
const float  FLOAT_INFINITY = 1e30;                 // бесконечность
const float  FLT_INFINITY = 1e30;                   // 
const float  FLOAT_EPSILON = 1.192092896e-07;       // минимальное число, для которого 
                                                    // выполняется условие 1.0f + FLOAT_EPSILON != 1.0f



void    MathInit( void );           // init
void    MathRelease( void );        // release



float   isqrt1f( float x );
float   sqrt1f( float x );

float   sin1f( float a );
float   cos1f( float a );
void    sincosf( float a, float* s, float* c );
float   tan1f( float a );

float   asin1f( float a );
float   acos1f( float a );
float   atan1f( float a );
float   atan2f( float y, float x );

float   pow2f( float x, float y );
float   exp1f( float f );
float   log1f( float f );

int     pow2i( int x, int y );

float   min2f( float a, float b );
int     min2i( int a, int b );
float   max2f( float a, float b );
int     max2i( int a, int b );

int     abs1i( int x );
float   abs1f( float f );

float   floor1f( float f );
float   ceil1f( float f );
float   round1f( float f );
float   trunc1f( float f );
float   frac1f( float f );

int     clamp3i( int min, int max, int val );
float   clamp3f( float min, float max, float val );

int     lerpi( int a, int b, float scale );
float   lerpf( float a, float b, float scale );



#endif //__MATH_H__