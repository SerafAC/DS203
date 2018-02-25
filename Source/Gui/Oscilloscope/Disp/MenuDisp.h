#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_DISP_MENUDISP_H
#define DSO_GUI_OSCILLOSCOPE_DISP_MENUDISP_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemDisp.h"

class CWndMenuDisplay : public CWnd {
 public:
  // Menu items
  CProviderEnum m_proAxes;
  CProviderEnum m_proDraw;
  CProviderEnum m_proAverage;
  CProviderEnum m_proPersist;
  CProviderEnum m_proGrid;
  CProviderEnum m_proAxis;

  CMPItem m_itmAxes;
  CMPItem m_itmDraw;
  CMPItem m_itmAverage;
  CMPItem m_itmPersist;
  CMPItem m_itmGrid;
  CMPItem m_itmAxis;

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif