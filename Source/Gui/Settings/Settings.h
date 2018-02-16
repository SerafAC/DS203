#pragma once
#ifndef DSO_GUI_SETTINGS_SETTINGS_H
#define DSO_GUI_SETTINGS_SETTINGS_H
#include "Controls/Slider.h"
#include "Core/SettingsCore.h"
#include "ItemAutoOff.h"
#include "ItemBacklight.h"
#include "ItemVolume.h"

class CWndMenuSettings : public CWnd {
public:
  CMIBacklight m_itmBacklight;
  CMIVolume m_itmVolume;
  CMIAutoOff m_itmAutoOff;
  CWndMenuItem m_itmReboot;

  virtual void Create(CWnd *pParent, ui16 dwFlags) {
    CWnd::Create("CWndMenuSettings", dwFlags, CRect(60, 60, 280, 240), pParent);
    m_itmBacklight.Create("Backlight", RGB565(808080), 2, this);
    m_itmVolume.Create("Volume", RGB565(808080), 2, this);
    m_itmAutoOff.Create("Enter standby mode", RGB565(808080), 2, this);
    // m_itmReboot.Create("Reboot to DFU", RGB565(ff8080), 2, this);
  }

  virtual void OnMessage(CWnd *pSender, CodeParam code, DataParam data) {
    // if ( code == ToWord('m', 'o') && pSender == &m_itmReboot )
    //	BIOS::SYS::Execute( BIOS::SYS::EDfu );
  }
};
#endif
