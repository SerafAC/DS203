#pragma once
#ifndef DSO_USER_THERMOMETER_H
#define DSO_USER_THERMOMETER_H
//#define ENABLE_THERMOMETER

#if defined(ADD_MODULE) && defined(ENABLE_THERMOMETER)
ADD_MODULE("Thermometer", CWndThermometer)
#endif

#ifndef __THERMOMETER_H__
#define __THERMOMETER_H__

#include <Source/Core/Utils.h>
#include <Source/Framework/Wnd.h>

class CWndThermometer : public CWnd {
  static const unsigned char bitmapThermometer[];

public:
  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnPaint();
  virtual void OnTimer();
  virtual void OnMessage(CWnd *pSender, ui16 code, ui32 data);
  virtual void OnKey(ui16 nKey);
};

#endif
#endif
