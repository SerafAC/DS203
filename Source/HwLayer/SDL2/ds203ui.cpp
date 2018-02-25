#include <SDL.h>
#include <Source/Framework/Application.h>
#include <Source/HwLayer/Bios.h>
#include <Source/HwLayer/SDL2/device.h>
#include <Source/HwLayer/Types.h>

CDevice *CDevice::m_pInstance = NULL;

template <class T, void DTOR(T *)>
class SdlItem {
  T *m_item;

 public:
  SdlItem(T *n = NULL) : m_item(n) {}
  ~SdlItem() {
    if (m_item) DTOR(m_item);
  }
  SdlItem &operator=(T *m) {
    if (m_item && (m_item != m)) DTOR(m_item);
    m_item = m;
    return *this;
  }
  operator T *() const { return m_item; }
  T *operator->() const { return m_item; }
};

typedef SdlItem<SDL_Window, SDL_DestroyWindow> SdlWindow;
typedef SdlItem<SDL_Renderer, SDL_DestroyRenderer> SdlRenderer;
typedef SdlItem<SDL_Texture, SDL_DestroyTexture> SdlTexture;
typedef SdlItem<SDL_Surface, SDL_FreeSurface> SdlSurface;

class SdlDevice : public CDevice {
  SdlWindow m_win;
  SdlSurface m_surface = SDL_LoadBMP("hello.bmp");
  SDL_Surface *m_screen;

 public:
  SdlDevice() {
    m_win = SDL_CreateWindow("Hello World", 0, 0, CDevice::Width,
                             CDevice::Height, 0);
    m_surface =
        SDL_CreateRGBSurface(0, CDevice::Width, CDevice::Height, 32, 0x00ff0000,
                             0x0000ff00, 0x000000ff, 0x00000000);
    SDL_LockSurface(m_surface);
    m_screen = SDL_GetWindowSurface(m_win);
    m_pInstance = this;
    keys = 0;
    mousex = -1;
    mousey = -1;
    moused = 0;
  }
  ~SdlDevice() { SDL_UnlockSurface(m_surface); }
  void operator()() {}
  void Blit() {
    SDL_UnlockSurface(m_surface);
    SDL_BlitSurface(m_surface, NULL, m_screen, NULL);
    SDL_UpdateWindowSurface(m_win);
    SDL_LockSurface(m_surface);
  }
  void proc1(int k, bool s) {
    if (s) {
      this->keys |= k;
    } else {
      this->keys &= ~k;
    }
  }
  void procKey(int k, bool s) {
    switch (k) {
      case SDLK_LEFT:
        proc1(BIOS::KEY::KeyLeft, s);
        break;
      case SDLK_RIGHT:
        proc1(BIOS::KEY::KeyRight, s);
        break;
      case SDLK_UP:
        proc1(BIOS::KEY::KeyUp, s);
        break;
      case SDLK_DOWN:
        proc1(BIOS::KEY::KeyDown, s);
        break;
      case SDLK_RETURN:
        proc1(BIOS::KEY::KeyEnter, s);
        break;
      case SDLK_BACKSPACE:
        proc1(BIOS::KEY::KeyEscape, s);
        break;
      case SDLK_SPACE:
        proc1(BIOS::KEY::KeyFunction, s);
        break;
      case SDLK_DELETE:
        proc1(BIOS::KEY::KeyFunction2, s);
        break;
      case SDLK_F1:
        proc1(BIOS::KEY::KeyS1, s);
        break;
      case SDLK_F2:
        proc1(BIOS::KEY::KeyS2, s);
        break;
    }
  }

  void OnKeyDown(int k) { procKey(k, true); }
  void OnKeyUp(int k) { procKey(k, false); }
  void OnMouseMove(int x, int y) {
    mousex = x;
    mousey = y;
  }
  void OnMouseDown() { moused = 1; }
  void OnMouseUp() { moused = 0; }
  void Loop() {
    current_buffer = (ui32 *)m_surface->pixels;
    CApplication app;
    app.Create();
    bool quit = false;
    int frame = 0;
    while (!quit) {
      SDL_Event e;
      if (SDL_PollEvent(&e)) {
        switch (e.type) {
          case SDL_KEYDOWN:
            OnKeyDown(e.key.keysym.sym);
            break;
          case SDL_KEYUP:
            OnKeyUp(e.key.keysym.sym);
            break;
          case SDL_MOUSEMOTION:
            OnMouseMove(e.motion.x, e.motion.y);
            break;
          case SDL_MOUSEBUTTONDOWN:
            OnMouseDown();
            break;
          case SDL_MOUSEBUTTONUP:
            OnMouseUp();
            break;
          case SDL_QUIT:
            quit = true;
            break;
        }
      }
      app();
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
