#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_CONTROLS_TIMEREF_H
#define DSO_GUI_OSCILLOSCOPE_CONTROLS_TIMEREF_H
#include <Source/Framework/Wnd.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphBase.h>

class CWndTimeReferences : public CWnd {
public:
  virtual void Create(CWnd *pParent, ui16 dwFlags) {
    CWnd::Create("CWndTimeReferences", dwFlags | CWnd::WsNoActivate,
                 CRect(12, 16, 12 + CWndGraph::DivsX * CWndGraph::BlkX, 21),
                 pParent);
  }

  virtual void OnPaint() {
    BIOS::LCD::Bar(m_rcClient, RGB565(000000));
    si16 x = (Settings.Trig.nTime - Settings.Time.Shift) /*>> 2*/;
    if (x >= 0 && x < m_rcClient.Width())
      BIOS::LCD::Draw(m_rcClient.left + x - 4, m_rcClient.top, RGB565(606060),
                      RGBTRANS, CShapes::trig_time);
  }
};
#endif
