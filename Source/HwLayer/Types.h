#ifndef HWLAYER_TYPES_H
#define HWLAYER_TYPES_H

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