#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_MATH_MENUMATH_H
#define DSO_GUI_OSCILLOSCOPE_MATH_MENUMATH_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemOperand.h"
#include "ItemOperator.h"
#include "ListOperand.h"
#include "ListOperator.h"

class CWndMenuMath : public CWnd {
  enum { A = 0, B = 1, C = 2 };

public:
  // Menu items
  CItemOperand m_itmOperand[3];
  CItemOperator m_itmOperator;

  CWndListOperand m_wndListOperand;
  CWndListOperator m_wndListOperator;

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif