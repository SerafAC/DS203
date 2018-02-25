#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_CONTROLS_GRAPHOSC_H
#define DSO_GUI_OSCILLOSCOPE_CONTROLS_GRAPHOSC_H

#include <Source/Core/Settings.h>
#include <Source/Gui/Oscilloscope/Math/ChannelMath.h>
#include "GraphBase.h"

class CWndOscGraph : public CWndGraph, public CMathChannel {
  ui16 m_arrAverageBuf[DivsX * BlkX];
  bool m_bPersistReset;
  bool m_bNeedRedraw;

  void _PrepareColumn(ui16 *column, ui16 n, ui16 clr);
  ui16 _Interpolate(ui16 clrA, ui16 clrB);

 public:
  CWndOscGraph();
  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
  virtual void OnPaint();
  void SetupMarkers(CSettings::Calibrator::FastCalc &Ch1fast,
                    CSettings::Calibrator::FastCalc &Ch2fast, int &nMarkerT1,
                    int &nMarkerT2, int &nMarkerY1, int &nMarkerY2);
  void SetupSelection(bool &bSelection, int &nMarkerT1, int &nMarkerT2);
  void GetCurrentRange(int &nBegin, int &nEnd);
  void ClearAverage();
  void ClearPersist();

  void OnPaintTY();
  void OnPaintXY();
};

#endif