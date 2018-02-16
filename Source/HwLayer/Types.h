#ifndef HWLAYER_TYPES_H
#define HWLAYER_TYPES_H
#include <stdint.h>
#include <string.h>

typedef uint8_t  ui8;
typedef int8_t   si8;
typedef uint16_t ui16;
typedef int16_t  si16;
typedef uint32_t ui32;
typedef int32_t  si32;

typedef void* PVOID;
typedef float FLOAT;
typedef int INT;
typedef const char * PCSTR;
typedef char * PSTR;
typedef char CHAR;
typedef ui32 UINT;

typedef uintptr_t CodeParam;
typedef uintptr_t DataParam;

template<int sz> struct sized_int; 
template<> struct sized_int<1> { typedef si8 type;}; 
template<> struct sized_int<2> { typedef si16 type;}; 
template<> struct sized_int<4> { typedef si32 type;}; 
namespace 
{
enum dummy {}; 
}
typedef sized_int<sizeof(dummy)>::type NATIVEENUM;


extern void AssertFailed(const char* file, int line, const char *expr);
#ifdef _ASSERT
# undef _ASSERT
#endif
#define _STR2(X) #X
#define _STR(X) _STR2(X)
#define _ASSERT(EXPR) if(!(EXPR)) { AssertFailed(__FILE__ ,__LINE__,""); }

template<class A, class B>
auto min(const A& a, const B& b) -> decltype(a<b?a:b) { return (a<b?a:b); }
template<class A, class B>
auto max(const A& a, const B& b) -> decltype(a>b?a:b) { return (a>b?a:b); }
template<class T>
T abs(const T& a) { return (a>0?a:-a); }

#define ToWord(a, b) (ui16)(((a)<<8)|(b))
#define ToDword(a, b, c, d) (ui32)((ToWord(d, c)<<16)|ToWord(b,a))

#define RGB565RGB(r, g, b) (((r)>>3)|(((g)>>2)<<5)|(((b)>>3)<<11))
#define Get565R(rgb) (((rgb)&0x1f)<<3)
#define Get565G(rgb) ((((rgb)>>5)&0x3f)<<2)
#define Get565B(rgb) ((((rgb)>>11)&0x1f)<<3)
#define GetHtmlR(rgb) ((rgb) >> 16)
#define GetHtmlG(rgb) (((rgb) >> 8)&0xff)
#define GetHtmlB(rgb) ((rgb)&0xff)
#define GetColorR(rgb) ((rgb) & 0xff)
#define GetColorG(rgb) (((rgb) >> 8)&0xff)
#define GetColorB(rgb) (((rgb) >> 16)&0xff)

#define RGB565(rgb) RGB565RGB( GetHtmlR(0x##rgb), GetHtmlG(0x##rgb), GetHtmlB(0x##rgb))
#define COUNT(arr) (int)(sizeof(arr)/sizeof(arr[0]))
#define RGBTRANS (RGB565(ff00ff)-1)

#define RGB32(r,g,b) ((r) | ((g)<<8) | ((b)<<16))

#define DecEnum(e) *((ui8*)&(e))-=1
#define IncEnum(e) *((ui8*)&(e))+=1

#include <limits.h>
#define MAXUINT     UINT_MAX
#define MAXINT      INT_MAX
#define MININT      INT_MIN

#ifdef _WINDOWS
#	include "Win32/Types.inc"
#endif

#ifdef _SDL2
#	include "SDL2/Types.inc"
#endif

#ifdef _ARM
#	include "ArmM3/Types.inc"
#endif

#ifndef __TYPES_H__
#	error Platform not defined
#endif

template<class T, class S> 
inline T narrow_cast(S s) {
	T t = static_cast<T>(s);
	_ASSERT(static_cast<S>(t) == s);
	return t;
}
template<>
inline float narrow_cast(double s) {
	return static_cast<float>(s);
}


#endif
