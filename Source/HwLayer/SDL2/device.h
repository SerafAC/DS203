#ifndef SDL2_DEVICE
#define SDL2_DEVICE
#include <Source/Main/Application.h>

class CDevice {
public:
	static const int Height=240; 
	static const int Width=400;

	static CDevice* m_pInstance;
	//CFrameBuffer display;
	int keys;
	int keybuf[1000];
	int mousex, mousey, moused;

public:
	int* GetKeys()
	{
		return keybuf;
	}
	void* GetBuffer()
	{
		return NULL;
	}
};




#endif
