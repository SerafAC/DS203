#pragma once
#ifndef DSO_GUI_GENERATOR_MAIN_ITEMGENWAVEFORM_H
#define DSO_GUI_GENERATOR_MAIN_ITEMGENWAVEFORM_H

#include <Source/Core/Controls.h>
#include <Source/Core/ListItems.h>
#include <Source/Core/Settings.h>

class CItemGenWaveform : public CItemProvider {
  CProviderEnum m_proWave;

public:
  void Create(CWnd *pParent) {
    m_proWave.Create((const char **)CSettings::Generator::ppszTextWave,
                     (NATIVEENUM *)&Settings.Gen.Wave,
                     CSettings::Generator::_WaveMax);
    CItemProvider::Create(&m_proWave, "Waveform", RGB565(b00040), pParent);
  }
};

#endif
