#ifndef __GENERATOR_MENUMAINMODULATION_H__
#define __GENERATOR_MENUMAINMODULATION_H__

#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Framework/Wnd.h>
#include <Source/Gui/Generator/Core/CoreGenerator.h>

class CWndMenuGeneratorMod : public CWnd {
public:
  CWndMenuItem m_itm[5];

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif