#ifndef HWLAYER_TYPES_H
#define HWLAYER_TYPES_H
#include <stdint.h>

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
