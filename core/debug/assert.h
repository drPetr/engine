#ifndef __ASSERT_H__
#define __ASSERT_H__

void __impl_assert( int e, const char* expr, const char* file, int line );
void __impl_asserta( int e, const char* expr, const char* file, int line, const char* fmt, ... );
void __impl_verify( int e, const char* expr, const char* file, int line );
void __impl_verifya( int e, const char* expr, const char* file, int line, const char* fmt, ... );


#if defined(DEBUG) || defined(_DEBUG)
    #define assert(e) __impl_assert( (int)(!!(e)), #e, __FILE__, __LINE__ )
    #define asserta(e,a,...) __impl_asserta( (int)(!!(e)), #e, __FILE__, __LINE__, a, ##__VA_ARGS__ )
    #define assert_not_null(e) __impl_asserta( (int)(!!(e)), #e, __FILE__, __LINE__, "%s", "pointer cannot be null" )
    #define assert_is_null(e) __impl_asserta( (int)(!(e)), #e, __FILE__, __LINE__, "%s", "pointer must be null" )
#else
    #define assert(e)
    #define asserta(e,a,...)
    #define assert_not_null(e)
    #define assert_is_null(e)
#endif

#define verify(e) __impl_verify( (int)(!!(e)), #e, __FILE__, __LINE__ )
#define verifya(e,a,...) __impl_verifya( (int)(!!(e)), #e, __FILE__, __LINE__, a, ##__VA_ARGS__ )
#define verify_not_null(e) __impl_verifya( (int)(!!(e)), #e, __FILE__, __LINE__, "%s", "pointer cannot be null" )

#endif // __ASSERT_H__