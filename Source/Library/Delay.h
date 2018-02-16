#pragma once
#ifndef DSO_LIBRARY_DELAY_H
#define DSO_LIBRARY_DELAY_H
#include <Source/HwLayer/Bios.h>

class CDelay {
public:
  static void DelayUs(ui32 us) {
    us = us * 12;
    while (us--) {
      __asm__("");
    }
  }

  static void DelayMs(ui32 ms) {
    while (ms--) {
      ui32 us = 12000;
      while (us--) {
        __asm__("");
      }
    }
  }
};
#endif
