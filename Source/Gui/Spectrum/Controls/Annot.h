#pragma once
#ifndef DSO_GUI_SPECTRUM_CONTROLS_ANNOT_H
#define DSO_GUI_SPECTRUM_CONTROLS_ANNOT_H

#include <Source/Framework/Wnd.h>

class CWndSpecAnnotations : public CWnd {
 public:
  virtual void Create(CWnd *pParent, ui16 wFlags) {
    CWnd::Create("CWndSpecAnnotations", wFlags | CWnd::WsNoActivate,
                 CRect(0, 0, 1, 1), pParent);
  }

  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data) {}

  virtual void OnPaint();
};

#endif