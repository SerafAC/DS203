#pragma once
#ifndef DSO_GUI_SPECTRUM_MAIN_MENUSPECTMAIN_H
#define DSO_GUI_SPECTRUM_MAIN_MENUSPECTMAIN_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphBase.h>
#include <Source/Gui/Oscilloscope/Input/ItemAnalog.h>
#include <Source/Gui/Oscilloscope/Input/ItemTime.h>

#include "ItemDisplay.h"
#include "ItemWindow.h"

class CWndMenuSpectMain : public CWnd {
public:
  // Menu items
  CItemAnalog m_itmCH1;
  CItemAnalog m_itmCH2;
  CItemTime m_itmTime;

  CItemSpecWindow m_itmWindow;
  CItemSpecDisplay m_itmDisplay;
  CWndMenuItem m_itmMarker;

  CWndMenuSpectMain();

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif