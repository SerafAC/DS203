#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef _WIN32
#error Include the folder "HwLayer/ArmM3" only for target device application
#endif

#include <string.h>
#include <stdint.h>

#define NATIVEPTR ui32
#define NATIVEENUM unsigned char

#define ToWord(a, b) (ui16)(((a)<<8)|(b))
#define ToDword(a, b, c, d) (ui32)((ToWord(d, c)<<16)|ToWord(b,a))

//#define LINKERSECTION(sec) __attribute__((section(".extra")))
#define LINKERSECTION(sec) 

struct FILEINFO {
	enum {
		SectorSize = 512
	};
	ui16 pCluster[3];
	ui32 pDirAddr[1];
	ui8 nMode;
	ui8 nSectors;
};

#define DLLAPI 

#endif
