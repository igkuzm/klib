#ifdef STDINT_H
#define STDINT_H

#ifndef __STDC__
  #error Not a standard C compiler.
#endif

//#if defined(__STDC__)
//# define PREDEF_STANDARD_C_1989
//# if defined(__STDC_VERSION__)
//#  define PREDEF_STANDARD_C_1990
//#  if (__STDC_VERSION__ >= 199409L)
//#   define PREDEF_STANDARD_C_1994
//#  endif
//#  if (__STDC_VERSION__ >= 199901L)
//#   define PREDEF_STANDARD_C_1999
//#  endif
//#  if (__STDC_VERSION__ >= 201112L)
//#    define PREDEF_STANDARD_C_2011
//#  endif
//#  if (__STDC_VERSION__ >= 201710L)
//#   define PREDEF_STANDARD_C_2018
//#  endif
//# endif
//#endif //defined (__STDC__)

/*#ifdef PREDEF_STANDARD_C_1999*/
#if __STDC_VERSION__ >= 199901L //for c99
#include <stdint.h>
#include <stdbool.h>
#else
typedef unsigned char  uint8_t;
typedef signed char    int8_t;
typedef unsigned short uint16_t;
typedef signed short   int16_t;
typedef unsigned long  uint32_t;
typedef signed long    int32_t;
typedef struct {int a; int b;} int64_t;
typedef	int64_t uint64_t;
typedef char bool;
#define true 1
#define false 0

#endif

#endif // STDINT_H
