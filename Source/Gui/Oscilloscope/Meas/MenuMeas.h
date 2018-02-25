#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_MEAS_MENUMEAS_H
#define DSO_GUI_OSCILLOSCOPE_MEAS_MENUMEAS_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemMeas.h"
#include "ListMeas.h"

class CWndMenuMeas : public CWnd {
 public:
  // Menu items
  CItemMeas m_itmMeas[6];

  CWndListMeas m_wndListMeas;

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);

 private:
  void _UpdateAll();
};

#endif