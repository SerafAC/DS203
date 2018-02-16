#pragma once
#ifndef DSO_GUI_SETTINGS_ITEMVOLUME_H
#define DSO_GUI_SETTINGS_ITEMVOLUME_H
#include <Source/Core/Settings.h>
#include <Source/Gui/Settings/Controls/Slider.h>
#include <Source/Gui/Settings/Core/SettingsCore.h>

class CMIVolume : public CMIProgress {
public:
  virtual int GetValue() { return Settings.Runtime.m_nVolume / 4; }
  virtual void SetValue(int nValue) {
    Settings.Runtime.m_nVolume = nValue * 4;
    CCoreSettings::Update();
    BIOS::SYS::Beep(100);
    Settings.Kick();
  }
  virtual int GetMin() { return 0; }
  virtual int GetMax() { return 25; }
};
#endif
