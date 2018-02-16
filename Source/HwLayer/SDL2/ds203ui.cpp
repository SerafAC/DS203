#include <SDL.h>
#include <Source/HwLayer/SDL2/device.h>
#include <Source/Framework/Application.h>

CApplication myApp;
CApplicationProto* g_app = &myApp;
CDevice *CDevice::m_pInstance=NULL; 

template<class T, void DTOR(T*)>
class SdlItem
{
	T *m_item;
	public:
	SdlItem(T *n=NULL) : m_item(n) {}
	~SdlItem() 
	{
		if (m_item) DTOR(m_item);
	}
	SdlItem & operator = (T *m) 
	{
		if (m_item) DTOR(m_item); 
		m_item=m;
	}
	operator T* () const { return m_item; }
	T* operator -> () const { return m_item; }
}; 

typedef SdlItem<SDL_Window,SDL_DestroyWindow> SdlWindow; 
typedef SdlItem<SDL_Renderer,SDL_DestroyRenderer> SdlRenderer; 
typedef SdlItem<SDL_Texture,SDL_DestroyTexture> SdlTexture; 
typedef SdlItem<SDL_Surface,SDL_FreeSurface> SdlSurface; 


class SdlDevice: public CDevice {
	SdlWindow m_win; 
    SdlSurface m_surface = SDL_LoadBMP("hello.bmp");
    SDL_Surface *m_screen; 
public:
	SdlDevice()
	{
		m_win=SDL_CreateWindow("Hello World", 0,0 , CDevice::Width, CDevice::Height, 0);
		m_surface = SDL_CreateRGBSurface(0,CDevice::Width, CDevice::Height,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
		SDL_LockSurface(m_surface);
		m_screen=SDL_GetWindowSurface(m_win); 
		m_pInstance = this;
		keys = 0;
		memset(keybuf, 0, sizeof(keybuf));
		mousex = -1;
		mousey = -1;
		moused = 0;
	}
	~SdlDevice()
	{
		SDL_UnlockSurface(m_surface);
	}
	void operator ()()
	{
	}
	void Blit( )
	{
		SDL_UnlockSurface(m_surface);
			SDL_BlitSurface(m_surface,NULL,m_screen,NULL);
			SDL_UpdateWindowSurface(m_win);
		SDL_LockSurface(m_surface);
	}
	void OnKeyDown(int k)
	{
		_ASSERT( k >= 0 && k < 1000 );
		keybuf[k] = 1;
		//keys |= k;
	}
	void OnKeyUp(int k)
	{
		_ASSERT( k >= 0 && k < 1000 );
		keybuf[k] = 0;
		//keys |= k;
	}
	void OnMouseMove(int x, int y)
	{
		mousex = x;
		mousey = y;
	}
	void OnMouseDown()
	{
		moused = 1;
	}
	void OnMouseUp()
	{
		moused = 0;
	}
	void Loop() 
	{
		bool quit=false;
		int frame=0;
		while (!quit) {
			SDL_Event e;
			if (SDL_PollEvent(&e)) {
				switch(e.type)
				{
					case SDL_KEYDOWN: 
						OnKeyDown(e.key.keysym.sym);
						break;
					case SDL_KEYUP: 
						OnKeyUp(e.key.keysym.sym);
						break;
					case SDL_MOUSEMOTION:
						OnMouseMove(e.motion.x,e.motion.y);
						break;
					case SDL_MOUSEBUTTONDOWN:
						OnMouseDown(); 
						break; 
					case SDL_MOUSEBUTTONUP:
						OnMouseUp(); 
						break; 
					case SDL_QUIT:
						quit=true;
						break; 
					}
			}
			ui32 *p=(ui32*)m_surface->pixels; 
			for (int i=0;i<40000;i++) 
			{
				p[i]=i+256*frame;
			}
			frame++;
			Blit();
		}
    }
};



int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
	SdlDevice dev; 
	dev.Loop();	
    SDL_Quit();
    return 0;
}


#if false 

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
const TCHAR szTitle[]=_T("DSO");					// The title bar text
const TCHAR szWindowClass[]=_T("DSO-Class");			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

#include <crtdbg.h>


HWND g_hwnd = NULL;
BOOL g_running = FALSE;

DWORD WINAPI ThreadProcDraw(HANDLE handle) 
{
	while ( !g_hwnd )
		Sleep( 10 );

	ShowWindow( g_hwnd, SW_SHOW );
	HDC hdc = GetDC( g_hwnd );

	while (g_running) 
	{
		g_dev.Blit( hdc );
		Sleep( 10 );
	}
	return 0;
}

DWORD WINAPI ThreadProcApp(HANDLE handle) 
{
	while ( !g_hwnd )
		Sleep( 10 );

	g_app->Create();
	while (g_running) 
	{
		g_dev();
		(*g_app)();	// contains sleep(1);
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;

	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_DS203UI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DS203UI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_DS203UI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	int nWidth = 400 + GetSystemMetrics(SM_CXFRAME)*2;
	int nHeight = 240 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME)*2;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	g_hwnd = hWnd;
	
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, CodeParam, DataParam)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
#include <stdio.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HANDLE hDrawThread = NULL;
	static HANDLE hAppThread = NULL;

	switch (message)
	{
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		g_dev.OnKeyDown( (int)wParam );
		break;

	case WM_KEYUP:
		g_dev.OnKeyUp( (int)wParam );
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		g_dev.Blit( hdc );
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
	{
		g_running = TRUE;
		hDrawThread = CreateThread( NULL, 0, &ThreadProcDraw, NULL, 0, NULL );
		hAppThread = CreateThread( NULL, 0, &ThreadProcApp, NULL, 0, NULL );
		break;
	}

	case WM_CLOSE:
		g_running = FALSE;
		Sleep(10);
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		TerminateThread( hDrawThread, 1 );
		TerminateThread( hAppThread, 1 );
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		g_dev.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONDOWN:
		g_dev.OnMouseDown();
		break;

	case WM_LBUTTONUP:
		g_dev.OnMouseUp();
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

#endif
