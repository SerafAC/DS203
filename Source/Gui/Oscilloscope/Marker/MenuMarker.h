#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_MARKER_MENUMARKER_H
#define DSO_GUI_OSCILLOSCOPE_MARKER_MENUMARKER_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Controls/GraphOsc.h>

#include "ItemDelta.h"
#include "ItemMarker.h"
#include "ListMarker.h"

class CWndMenuCursor : public CWnd {
 public:
  // Menu items
  CItemMarker m_itmX1;
  CItemMarker m_itmX2;
  CItemMarker m_itmY1;
  CItemMarker m_itmY2;
  CItemMarkerDelta m_itmDeltaX;
  CItemMarkerDelta m_itmDeltaY;

  CWndListMarker m_wndListCursor;

  CWndMenuCursor();

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);

 private:
  void Find(CSettings::Marker *pMarker, CSettings::Marker::EFind mode);
};

#endif