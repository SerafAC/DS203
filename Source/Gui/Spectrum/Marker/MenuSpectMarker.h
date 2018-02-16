#ifndef __MENUSPECTMARKER_H__
#define __MENUSPECTMARKER_H__

#include "ItemMarker.h"
#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>

class CWndMenuSpectMarker : public CWnd {
public:
  // Menu items
  CWndMenuItem m_itmSource;
  CWndMenuItem m_itmTrack;
  CItemSpecMarker m_itmValue;

  CWndMenuSpectMarker();

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif