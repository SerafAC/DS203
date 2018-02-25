#pragma once
#ifndef DSO_HWLAYER_SDL2_DEVICE_H
#define DSO_HWLAYER_SDL2_DEVICE_H
#include <Source/Main/Application.h>

class CDevice {
 public:
  static const int Height = 240;
  static const int Width = 400;

  static CDevice *m_pInstance;
  // CFrameBuffer display;
  int keys;
  int mousex, mousey, moused;
  void *current_buffer = NULL;

 public:
  int GetKeys() { return keys; }
  void *GetBuffer() { return current_buffer; }
};

#endif
