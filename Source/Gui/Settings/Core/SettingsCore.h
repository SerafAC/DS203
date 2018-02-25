#pragma once
#ifndef DSO_GUI_SETTINGS_CORE_SETTINGSCORE_H
#define DSO_GUI_SETTINGS_CORE_SETTINGSCORE_H
#include <Source/Core/Settings.h>
#include <Source/Framework/Classes.h>
#include <Source/HwLayer/Types.h>

class CCoreSettings {
 public:
  static void Update() {
    BIOS::SYS::SetBacklight(Settings.Runtime.m_nBacklight);
    BIOS::SYS::SetVolume(Settings.Runtime.m_nVolume);
  }
};

#endif
