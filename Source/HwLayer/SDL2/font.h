#pragma once

#include <Source/HwLayer/Common/font-bits.h>

const ui8 *GetFont(ui8 Code) { return font_bits + (Code * 14); }
