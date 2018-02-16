#pragma once
#ifndef DSO_GUI_GENERATOR_EDIT_MENUGENEDIT_H
#define DSO_GUI_GENERATOR_EDIT_MENUGENEDIT_H

#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Framework/Wnd.h>
#include <Source/Gui/Generator/Core/CoreGenerator.h>

class CWndMenuGeneratorEdit : public CWnd {
public:
  CWndMenuItem m_itmCopy;
  CWndMenuItem m_itmNormalize;
  CWndMenuItem m_itmInvert;
  CWndMenuItem m_itmEdit;
  CWndMenuItem m_itmSave;
  CWndMenuItem m_itmLoad;

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif