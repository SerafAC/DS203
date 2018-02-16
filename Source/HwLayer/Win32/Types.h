#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef _WIN32
#error Include the folder "HwLayer/Win32" only for desktop application compilation
#endif

#define LINKERSECTION(sec) 
#define __asm__(x) 

#include <stdio.h>
struct FILEINFO {
	enum {
		SectorSize = 512
	};
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
