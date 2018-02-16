#pragma once
#ifndef DSO_HWLAYER_SDL2_FONT_H
#define DSO_HWLAYER_SDL2_FONT_H

#include <Source/HwLayer/Common/font-bits.h>
#include <Source/HwLayer/Types.h>
const ui8 *GetFont(ui8 Code) { return font_bits + (Code * 14); }
#endif
