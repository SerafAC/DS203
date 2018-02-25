#pragma once
#ifndef DSO_FRAMEWORK_THREAD_H
#define DSO_FRAMEWORK_THREAD_H

#include <Source/HwLayer/Bios.h>
#include <Source/Main/Application.h>

class CThread {
 public:
  virtual void Start() { _ASSERT(0); }
  virtual void Stop() { _ASSERT(0); }
  virtual void Run() { _ASSERT(0); }
  virtual bool IsRunning() {
    _ASSERT(0);
    return false;
  }
  virtual int GetResult() {
    _ASSERT(0);
    return 0;
  }
  void Sleep(int nTime) {
    ui32 nLastTime = BIOS::SYS::GetTick() + nTime;
    do {
      Application.operator()();
    } while (BIOS::SYS::GetTick() < nLastTime);
  }
};

#endif
