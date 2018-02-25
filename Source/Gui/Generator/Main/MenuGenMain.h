#pragma once
#ifndef DSO_GUI_GENERATOR_MAIN_MENUGENMAIN_H
#define DSO_GUI_GENERATOR_MAIN_MENUGENMAIN_H

//#include <Source/HwLayer/bios.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>
#include <Source/Framework/Wnd.h>
#include <Source/Gui/Generator/Core/CoreGenerator.h>

#include "ItemGenAmplitude.h"
#include "ItemGenDuty.h"
#include "ItemGenFrequency.h"
#include "ItemGenOffset.h"
#include "ItemGenWaveform.h"

class CWndMenuGenerator : public CCoreGenerator, public CWnd {
 public:
  CItemGenWaveform m_itmWave;
  CItemFrequency m_itmFreq;
  CItemAmplitude m_itmAmpl;
  CItemOffset m_itmOffset;
  CItemDuty m_itmDuty;

  virtual void Create(CWnd *pParent, ui16 dwFlags);
  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data);
};

#endif