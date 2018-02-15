#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef _WIN32
#error Include the folder "HwLayer/Win32" only for desktop application compilation
#endif

#include <stdio.h>
#if false
#include <windows.h>
#include <crtdbg.h>
#include <math.h>
#endif

#define NATIVEPTR uintptr_t
#define NATIVEENUM long

#define ToWord(a, b) (ui16)(((a)<<8)|(b))
#define ToDword(a, b, c, d) (ui32)((ToWord(d, c)<<16)|ToWord(b,a))

#define LINKERSECTION(sec) 
#define __asm__(x) 
//__attribute__((section(".extra")))
/*
#ifndef _WIN32
__attribute__((section(".extra")))
#endif
*/

struct FILEINFO {
	enum {
		SectorSize = 512
	};
	/*
	ui16 pCluster[3];
	ui32 pDirAddr[1];
	*/
	ui8 nMode;
	ui8 nSectors;
	FILE *f;
};

#ifdef _DLLEXPORT
#	define DLLAPI __declspec(dllexport)
#else
#	define DLLAPI
#endif

#endif
