#pragma once
#ifndef DSO_MAIN_APPLICATION_H
#define DSO_MAIN_APPLICATION_H

#ifndef __APPLICATION__
#define __APPLICATION__

#include <Source/Framework/Application.h>

#define Application (*CApplication::getInstance())

class CApplication : public CApplicationProto {
  static CApplication *m_pInstance;

 public:
  static CApplication *getInstance();

  CApplication();
  ~CApplication();
  virtual void Create();
  virtual void Destroy();
  virtual bool operator()();
};

#endif
#endif
