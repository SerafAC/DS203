#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_MATH_ITEMOPERATOR_H
#define DSO_GUI_OSCILLOSCOPE_MATH_ITEMOPERATOR_H
#include <Source/Core/Controls.h>
#include <Source/Core/Settings.h>

class CItemOperator : public CWndMenuItem {
 public:
  CSettings::MathOperator *m_pOper;

 public:
  virtual void Create(CSettings::MathOperator *pOper, CWnd *pParent) {
    _ASSERT(pOper);
    m_pOper = pOper;
    CWndMenuItem::Create(NULL, pOper->uiColor, 2, pParent);
    SetColorPtr(&pOper->uiColor);
  }

  virtual void OnPaint() {
    bool bEnabled = m_pOper->Type != CSettings::MathOperator::_Off;
    ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);

    CWndMenuItem::OnPaint();

    int x = m_rcClient.left + 10 + MarginLeft;
    int y = m_rcClient.top;
    BIOS::LCD::Print(x, y, clr, RGBTRANS, "Operation");
    y += 16;
    BIOS::LCD::Print(x, y, clr, RGBTRANS,
                     CSettings::MathOperator::ppszTextType[(int)m_pOper->Type]);
  }

  virtual void OnKey(ui16 nKey) {
    _ASSERT(m_pOper);
    if (nKey & BIOS::KEY::KeyEnter) {
      SendMessage(m_pParent, ToWord('m', 'c'), m_pOper);
      return;
    }
    if (nKey & BIOS::KEY::KeyLeft && m_pOper->Type > 0) {
      _ASSERT(sizeof(m_pOper->Type) == sizeof(NATIVEENUM));
      DecEnum(m_pOper->Type);
      Invalidate();
      SendMessage(m_pParent, ToWord('i', 'u'), m_pOper);
    }
    if (nKey & BIOS::KEY::KeyRight &&
        m_pOper->Type < CSettings::MathOperator::_TypeMax) {
      _ASSERT(sizeof(m_pOper->Type) == sizeof(NATIVEENUM));
      IncEnum(m_pOper->Type);
      Invalidate();
      SendMessage(m_pParent, ToWord('i', 'u'), m_pOper);
    }

    CWnd::OnKey(nKey);
  }
};

#endif
