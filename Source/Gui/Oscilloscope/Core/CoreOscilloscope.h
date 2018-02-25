#pragma once
#ifndef DSO_GUI_OSCILLOSCOPE_CORE_COREOSCILLOSCOPE_H
#define DSO_GUI_OSCILLOSCOPE_CORE_COREOSCILLOSCOPE_H

#include <Source/Framework/Classes.h>
#include <Source/HwLayer/Types.h>

class CCoreOscilloscope {
 public:
  static void ConfigureAdc();
  static void ConfigureTrigger();
  static void UpdateConstants();
  // mask
  static void GetMaskAt(int nX, ui8 **pLow, ui8 **pHigh);
  static void GetMaskStats(int **nPass, int **nFail);
};

#endif