#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_INPUT_MENUINPUT_H
#define DSO_GUI_OSCILLOSCOPE_INPUT_MENUINPUT_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemAnalog.h"
#include "ItemDigital.h"
#include "ItemTime.h"
#include "ItemTrigger.h"
#include "ItemWindow.h"
#include "ListAnalog.h"
#include "ListDigital.h"
#include "ListTrigger.h"

class CWndMenuInput : public CWnd {
 public:
  // Menu items
  CItemAnalog m_itmCH1;
  CItemAnalog m_itmCH2;
  CItemDigital m_itmCH3;
  CItemDigital m_itmCH4;
  CItemTime m_itmTime;
  CItemWindow m_itmWindow;
  CItemTrigger m_itmTrig;

  // Lists
  CWndListAInput m_wndListAInput;
  CWndListDInput m_wndListDInput;
  CComboSelector m_wndComboSelector;
  CWndListTrigger m_wndListTrigger;

  CWndMenuInput();

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif