#ifndef HWLAYER_TYPES_H
#define HWLAYER_TYPES_H
#include <stdint.h>

typedef uint8_t  ui8;
typedef int8_t   si8;
typedef uint16_t ui16;
typedef int16_t  si16;
typedef uint32_t ui32;
typedef int32_t  si32;

#ifndef min 
#	define min(a, b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#	define max(a, b) ((a)>(b)?(a):(b))
#endif
#ifndef ABC
#	define ABS(a) ((a)>0?(a):(-(a)))
#endif

#ifdef _WINDOWS
#	include "Win32/Types.h"
#endif

#ifdef _ARM
#	include "ArmM3/Types.h"
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
