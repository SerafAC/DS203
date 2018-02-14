#ifndef HWLAYER_TYPES_H
#define HWLAYER_TYPES_H
#include <stdint.h>

template<class T, class X> T narrow_cast(const X &x) { return (T)(x); }
typedef void(*voidfun_t)();

#ifdef _WINDOWS
#	include "Win32/Types.h"
#endif

#ifdef _ARM
#	include "ArmM3/Types.h"
#endif

#ifndef __TYPES_H__
#	error Platform not defined
#endif

#endif