#include "device.h"
#include <SDL.h>
#include <SDL_keycode.h>
#include <Source/HwLayer/Bios.h>
#include <Source/HwLayer/Types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <errno.h>
typedef int errno_t;
static errno_t fopen_s(FILE **f, const char *name, const char *mode) {
  *f = fopen(name, mode);
  if (!*f)
    return errno;
  return 0;
}

static void Sleep(int ms) { SDL_Delay(ms); }
static ui32 GetTickCount() { return SDL_GetTicks(); }
#endif

#define DEVICE CDevice::m_pInstance
#include "font.h"
#include "sprintf/spf.h"
#include <math.h>

CRect m_rcBuffer;
CPoint m_cpBuffer;
int m_nKeys;
int g_nBufferLen = 4096;

int _DrawChar(int x, int y, unsigned short clrf, unsigned short clrb, char ch);
ui32 FROM_565_TO_RGB(unsigned short clr565);
ui16 FROM_RGB_TO_565(unsigned int clrrgb);
ui8 _Round(int x, int y);

void AssertFailed(const char *location, int line, const char *expr) {
  fprintf(stderr, "%s(%d): Assertion '%s' failed", location, line, expr);
  abort();
}

void BIOS::SYS::Init() {}

int BIOS::LCD::Printf(int x, int y, unsigned short clrf, unsigned short clrb,
                      const char *format, ...) {
  char buffer[256];
  /*
  va_list args;
  va_start (args, format);
  vsnprintf_s (buffer, 256, 255, format, args);
  int nAux = Print (x, y, clrf, clrb, buffer);
  va_end (args);
  */

  char *bbuf = buffer;
  va_list args;
  va_start(args, format);
  int nAux = print(&bbuf, format, args);
  Print(x, y, clrf, clrb, buffer);

  return nAux;
}

int BIOS::LCD::Print(int x, int y, unsigned short clrf, unsigned short clrb,
                     char *str) {
  if (!str || !*str)
    return 0;
  int nAux = 0;
  int _x = x;
  for (; *str; str++) {
    if (*str == '\n') {
      x = _x;
      y += 16;
      continue;
    }
    int nW = _DrawChar(x, y, clrf, clrb, *str);
    x += nW;
    nAux += nW;
  }
  return nAux;
}

int BIOS::LCD::Print(int x, int y, unsigned short clrf, unsigned short clrb,
                     const char *str) {
  /*
#ifdef _WIN32
  _ASSERT( (int)str > 0x10000 );
  const char *xx = str;
  xx=xx;
#endif*/
  if (!str || !*str)
    return 0;
  int nAux = 0;
  int _x = x;
  for (; *str; str++) {
    if (*str == '\n') {
      x = _x;
      y += 16;
      continue;
    }
    int nW = _DrawChar(x, y, clrf, clrb, *str);
    x += nW;
    nAux += nW;
  }
  return nAux;
}

int BIOS::LCD::Print(const CPoint &cp, unsigned short clrf, char *str) {
  return Print(cp.x, cp.y, clrf, RGBTRANS, str);
}

void BIOS::LCD::PutPixel(int x, int y, unsigned short clr) {
  if (clr == RGBTRANS)
    return;
  ui32 *pBuf = (ui32 *)DEVICE->GetBuffer();
  pBuf[y * CDevice::Width + x] = FROM_565_TO_RGB(clr);
}

ui16 BIOS::LCD::GetPixel(int x, int y) {
  ui32 *pBuf = (ui32 *)DEVICE->GetBuffer();
  return FROM_RGB_TO_565(pBuf[y * CDevice::Width + x]);
}

void BIOS::LCD::PutPixel(const CPoint &cp, unsigned short clr) {
  PutPixel(cp.x, cp.y, clr);
}

void BIOS::LCD::Clear(unsigned short clr) {
  ui32 *pBuf = (ui32 *)DEVICE->GetBuffer();
  ui32 c = FROM_565_TO_RGB(clr);
  for (int i = 0; i < CDevice::Width * CDevice::Height; i++, pBuf++)
    *pBuf = c;
}

void BIOS::LCD::RoundRect(int x1, int y1, int x2, int y2, unsigned short clr) {
  for (int x = x1; x < x2; x++)
    for (int y = y1; y < y2; y++)
      if (!_Round(min(x - x1, x2 - x - 1), min(y - y1, y2 - y - 1)))
        PutPixel(x, y, clr);
}

void BIOS::LCD::RoundRect(const CRect &rc, unsigned short clr) {
  RoundRect(rc.left, rc.top, rc.right, rc.bottom, clr);
}

void BIOS::LCD::Bar(int x1, int y1, int x2, int y2, unsigned short clr) {
  for (int x = x1; x < x2; x++)
    for (int y = y1; y < y2; y++)
      PutPixel(x, y, clr);
}

void BIOS::LCD::Bar(const CRect &rc, unsigned short clr) {
  Bar(rc.left, rc.top, rc.right, rc.bottom, clr);
}

void BIOS::LCD::Rectangle(const CRect &rc, unsigned short clr) {
  BIOS::LCD::Bar(rc.left, rc.top, rc.right, rc.top + 1, clr);
  BIOS::LCD::Bar(rc.left, rc.bottom - 1, rc.right, rc.bottom, clr);
  BIOS::LCD::Bar(rc.left, rc.top, rc.left + 1, rc.bottom, clr);
  BIOS::LCD::Bar(rc.right - 1, rc.top, rc.right, rc.bottom, clr);
}

void BIOS::LCD::Pattern(int x1, int y1, int x2, int y2, const ui16 *pat,
                        int l) {
  const ui16 *patb = pat;
  const ui16 *pate = patb + l;

  for (int x = x1; x < x2; x++)
    // for (int y=y1; y<y2; y++)
    for (int y = y2 - 1; y >= y1; y--) {
      PutPixel(x, y, *pat);
      if (++pat == pate)
        pat = patb;
    }
}

void BIOS::LCD::GetImage(const CRect &rcRect, ui16 *pBuffer) {
  ui32 *pBuf = (ui32 *)DEVICE->GetBuffer();
  int x1 = rcRect.left, x2 = rcRect.right, y1 = rcRect.top, y2 = rcRect.bottom;
  for (int x = x1; x < x2; x++)
    for (int y = y2 - 1; y >= y1; y--) {
      *pBuffer++ = FROM_RGB_TO_565(pBuf[y * CDevice::Width + x]);
    }
}

void BIOS::LCD::PutImage(const CRect &rcRect, ui16 *pBuffer) {
  int x1 = rcRect.left, x2 = rcRect.right, y1 = rcRect.top, y2 = rcRect.bottom;
  for (int x = x1; x < x2; x++)
    for (int y = y2 - 1; y >= y1; y--) {
      PutPixel(x, y, *pBuffer++);
    }
}

int BIOS::LCD::Draw(int x, int y, unsigned short clrf, unsigned short clrb,
                    const char *p) {
  int h = *p++;
  for (int _x = 0; _x < 80; _x++)
    for (int _y = 0; _y < h; _y++) {
      if (!p[_x * h + _y])
        return _x;
      PutPixel(x + _x, y + _y, (p[_x * h + _y] & 4) ? clrf : clrb);
    }
  return 8;
}

void BIOS::LCD::BufferBegin(const CRect &rc, ui8 nMode) {
  m_rcBuffer = rc;
  m_cpBuffer = rc.TopLeft();
}

void BIOS::LCD::BufferPush(ui16 clr) {
  PutPixel(m_cpBuffer, clr);
  if (++m_cpBuffer.x >= m_rcBuffer.right) {
    m_cpBuffer.x = m_rcBuffer.left;
    m_cpBuffer.y++;
  }
}

void BIOS::LCD::BufferEnd() {}

void BIOS::LCD::Shadow(int x1, int y1, int x2, int y2, unsigned int nColor) {
  // rrggbbaa
  int nA_ = nColor >> 24;
  int nR_ = (nColor >> 16) & 0xff;
  int nG_ = (nColor >> 8) & 0xff;
  int nB_ = nColor & 0xff;

  for (int x = x1; x < x2; x++)
    for (int y = y1; y < y2; y++)
      if (!_Round(min(x - x1, x2 - x - 1), min(y - y1, y2 - y - 1))) {
        ui16 nOld = GetPixel(x, y);
        int nR = Get565R(nOld);
        int nG = Get565G(nOld);
        int nB = Get565B(nOld);

        nR += ((nR_ - nR) * nA_) >> 8;
        nG += ((nG_ - nG) * nA_) >> 8;
        nB += ((nB_ - nB) * nA_) >> 8;

        PutPixel(x, y, RGB565RGB(nR, nG, nB));
      }
}

ui16 BIOS::KEY::GetKeys() { return DEVICE->GetKeys(); }

ui8 _Round(int x, int y) {
  const static ui8 r[] = {
      1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  if (x < 8 && y < 8)
    return r[y * 8 + x];
  return 0;
}

int _DrawChar(int x, int y, unsigned short clrf, unsigned short clrb, char ch) {
  const unsigned char *pFont = GetFont(ch);
  if (clrb == RGBTRANS) {
    for (ui8 _y = 0; _y < 14; _y++) {
      ui8 col = ~*pFont++;

      for (ui8 _x = 0; _x < 8; _x++, col <<= 1)
        if (col & 128)
          BIOS::LCD::PutPixel(x + _x, y + _y, clrf);
    }
  } else if (clrf == RGBTRANS) {
    for (ui8 _y = 0; _y < 14; _y++) {
      ui8 col = ~*pFont++;

      for (ui8 _x = 0; _x < 8; _x++, col <<= 1)
        if ((col & 128) == 0)
          BIOS::LCD::PutPixel(x + _x, y + _y, clrb);
    }
  } else {
    for (ui8 _y = 0; _y < 14; _y++) {
      ui8 col = ~*pFont++;

      for (ui8 _x = 0; _x < 8; _x++, col <<= 1)
        if (col & 128)
          BIOS::LCD::PutPixel(x + _x, y + _y, clrf);
        else
          BIOS::LCD::PutPixel(x + _x, y + _y, clrb);
    }
  }
  return 8;
}

ui32 FROM_565_TO_RGB(unsigned short clr565) {
  unsigned char b = clr565 & 0x1F;
  clr565 >>= 5;
  unsigned char g = clr565 & 0x3F;
  clr565 >>= 6;
  unsigned char r = (unsigned char)clr565;
  r = r * 255 / 0x1f;
  g = g * 255 / 0x3f;
  b = b * 255 / 0x1f;
  return RGB32(r, g, b);
}

ui16 FROM_RGB_TO_565(unsigned int clrrgb) {
  int b = clrrgb & 0xff;
  clrrgb >>= 8;
  int g = clrrgb & 0xff;
  clrrgb >>= 8;
  int r = clrrgb & 0xff;

  return RGB565RGB(r, g, b);
}

void BIOS::LCD::Buffer(int x, int y, unsigned short *pBuffer, int n) {
  y += n;
  while (n--)
    PutPixel(x, --y, *pBuffer++);
}

ui32 BIOS::SYS::GetTick() { return GetTickCount(); }

void BIOS::LCD::Line(int x1, int y1, int x2, int y2, unsigned short clr) {
  // if x1 == x2 or y1 == y2, then it does not matter what we set here
  si16 delta_x(x2 - x1);
  si8 ix((delta_x > 0) - (delta_x < 0));
  delta_x = abs(delta_x) << 1;

  si16 delta_y(y2 - y1);
  si8 iy((delta_y > 0) - (delta_y < 0));
  delta_y = abs(delta_y) << 1;

  PutPixel(x1, y1, clr);

  if (delta_x >= delta_y) {
    // error may go below zero
    si16 error(delta_y - (delta_x >> 1));

    while (x1 != x2) {
      if (error >= 0) {
        if (error || (ix > 0)) {
          y1 += iy;
          error -= delta_x;
        }
        // else do nothing
      }
      // else do nothing

      x1 += ix;
      error += delta_y;

      PutPixel(x1, y1, clr);
    }
  } else {
    // error may go below zero
    si16 error(delta_x - (delta_y >> 1));

    while (y1 != y2) {
      if (error >= 0) {
        if (error || (iy > 0)) {
          x1 += ix;
          error -= delta_y;
        }
        // else do nothing
      }
      // else do nothing

      y1 += iy;
      error += delta_x;
      PutPixel(x1, y1, clr);
    }
  }
}

// ADC
void BIOS::ADC::Init() {}

bool bADCEnabled = false;

void BIOS::ADC::Enable(bool bEnable) { bADCEnabled = bEnable; }

bool BIOS::ADC::Enabled() { return bADCEnabled; }

void BIOS::ADC::Configure(ui8 nACouple, ui8 nARange, ui16 nAOffset,
                          ui8 nBCouple, ui8 nBRange, ui16 nBPosition,
                          float fTimePerDiv) {}

bool bADCReady = false;
unsigned char BIOS::ADC::Ready() {
  static long lLast = 0;
  if (!bADCEnabled)
    return false;
  if (bADCReady)
    return bADCReady;
  long lTick = GetTickCount();
  if (lLast == 0)
    lLast = GetTickCount();
  bADCReady = (GetTickCount() - lLast) > 20;
  if (bADCReady)
    lLast = lTick;
  return bADCReady;
}

unsigned long BIOS::ADC::Get() {
  static long lCounter = 0;
  if (lCounter == 4096)
    lCounter = 0;

  FLOAT a, b;

  switch (4) {
  case 1: {
    FLOAT fm = narrow_cast<float>((sin(GetTickCount() * 0.001f) + 1.0f) * 0.5f);
    if (fm < 0.5)
      fm = 0.8f - 0.7f * fm;
    else
      fm = 0;
    FLOAT fa = 0.035f + (GetTickCount() % 20000) / 20000.0f * 0.14f;
    a = narrow_cast<float>(cos(lCounter * (fa)*10) * (fm) + 0.2f);
    b = narrow_cast<float>(sin(lCounter * 0.011f + 1) * 0.5f);

    break;
  }

  case 2: {
    FLOAT fm = 1;
    FLOAT fa = 0.035f; // + (GetTickCount()%20000)/20000.0f*0.14f;
    a = narrow_cast<float>(cos(lCounter * (fa)*10) * (fm) + 0.2f);
    b = narrow_cast<float>(sin(lCounter * 0.011f + 1) * 0.5f);
    break;
  }

  case 3: {
    a = 0;
    b = 0;
    float t = narrow_cast<float>(-pow(abs(cos(GetTickCount() * 0.001f)), 50));
    a = t / 3;
    break;
  }

  case 4: {
    FLOAT fm = 1;
    FLOAT fa = 0.05f;
    a = narrow_cast<float>(sin(lCounter * fa + 3.14 / 2) * 0.8);
    b = narrow_cast<float>(sin(lCounter * fa) * 0.25f);
    break;
  }
  }
  // fa = lCounter*0.0001f;
  unsigned long da = (ui32)((a + 1.0f) * 127);
  unsigned long db = (ui32)((b + 1.0f) * 127);
  da = max(0, min(da, 255));
  db = max(0, min(db, 255));
  da |= rand() & 3;
  lCounter++;
  return da | (db << 8);
}

#define ADCSIZE 4096
unsigned long g_ADCMem[ADCSIZE];

void BIOS::ADC::Copy(int nCount) {
  _ASSERT(nCount <= ADCSIZE);
  for (int i = 0; i < ADCSIZE; i++) {
    ui32 nOld = g_ADCMem[i] & 0xff000000;
    g_ADCMem[i] = (Get() & 0x00ffffff) | nOld;
  }
}

unsigned long BIOS::ADC::GetCount() { return ADCSIZE; }

BIOS::ADC::TSample &BIOS::ADC::GetAt(int i) {
  _ASSERT(i >= 0 && i < ADCSIZE);
  return g_ADCMem[i];
}

void BIOS::ADC::Restart() { bADCReady = false; }

void BIOS::GEN::ConfigureDc(ui16 nData) {}

void BIOS::GEN::ConfigureSq(ui16 psc, ui16 arr, ui16 ccr) {}

void BIOS::GEN::ConfigureWave(ui16 *pData, ui16 cnt) {}

void BIOS::GEN::ConfigureWaveRate(ui16 arr) {}

void BIOS::ADC::ConfigureTrigger(ui16 nTThreshold, ui16 nVThreshold,
                                 ui8 nSource, ui8 nType) {}

// void ADC::Configure(ui8 nACouple, ui8 nARange, ui16 nAOffset, ui8 nBCouple,
// ui8 nBRange, ui16 nBPosition, ui16 nTimePsc, ui16 nTimeArr)
//{
//}

PVOID BIOS::DSK::GetSharedBuffer() {
  static ui8 pSectorBuffer[FILEINFO::SectorSize];
  void *p = pSectorBuffer;
  return p;
}

bool BIOS::DSK::Open(FILEINFO *pFileInfo, const char *strName, ui8 nIoMode) {
  char name[32];
  memcpy(name, strName, 8);
  name[8] = 0;
  while (name[BIOS::UTIL::StrLen(name) - 1] == ' ')
    name[BIOS::UTIL::StrLen(name) - 1] = 0;
  BIOS::UTIL::StrCat(name, ".");
  BIOS::UTIL::StrCat(name, strName + 8);
  errno_t err;
  if (nIoMode == BIOS::DSK::IoRead)
    err = fopen_s(&pFileInfo->f, name, "rb");
  else if (nIoMode == BIOS::DSK::IoWrite)
    err = fopen_s(&pFileInfo->f, name, "wb");
  else
    return false;
  pFileInfo->nSectors = 0;
  return (err == 0);
}

bool BIOS::DSK::Read(FILEINFO *pFileInfo, ui8 *pSectorData) {
  return (fread(pSectorData, 512, 1, pFileInfo->f) == 1);
}

bool BIOS::DSK::Write(FILEINFO *pFileInfo, ui8 *pSectorData) {
  if (fwrite(pSectorData, 512, 1, pFileInfo->f) != 1)
    return false;
  pFileInfo->nSectors++;
  return true;
}

bool BIOS::DSK::Close(FILEINFO *pFileInfo, int nSize /*=-1*/) {
  if (pFileInfo->nMode == BIOS::DSK::IoWrite) {
    if (nSize != -1) {
      // not supported...
    }
  }
  fclose(pFileInfo->f);
  return true;
}

void BIOS::SYS::Beep(int) {}

void BIOS::DBG::Print(const char *format, ...) {
  _ASSERT(format);
  static int px = 0;
  static int py = 0;

  char buf[128];
  char *bbuf = buf;

  va_list args;

  va_start(args, format);
  print(&bbuf, format, args);

  fprintf(stderr, "%s", buf);
  for (bbuf = buf; *bbuf; bbuf++) {
    if (*bbuf == '\n' || px >= 400 - 8) {
      px = 0;
      py += 14;
      if (py > 240 - 16)
        py = 0;
      continue;
    }
    px += _DrawChar(px, py, RGB565(ffffff), RGB565(0000B0), *bbuf);
  }
}

#if 0
int BIOS::LCD::Print (int x, int y, unsigned short clrf, unsigned short clrb, const char *str)
{
	if (!str || !*str)
		return 0;
	int _x = x;
	for (;*str; str++)
	{
		if (*str == '\n')
		{
			x = _x;
			y += 16;
			continue;
		}
		x += _DrawChar(x, y, clrf, clrb, *str);
	}
	return x - _x;
}

#endif
int BIOS::DBG::sprintf(char *buf, const char *format, ...) {
  char *bbuf = buf;

  va_list args;

  va_start(args, format);
  return print(&bbuf, format, args);
}

void BIOS::SYS::DelayMs(unsigned short l) { Sleep(l); }

int BIOS::SYS::GetBattery() { return 85; }

void BIOS::SERIAL::Init() {}

void BIOS::SERIAL::Configure(int nBaudrate) {}

void BIOS::SERIAL::Send(const char *strBuf) {
  //	OutputDebugString( "Serial:" );
  fprintf(stderr, "%s\n", strBuf);
  //	OutputDebugString( "\n" );
}

const char *pserial = NULL;

int BIOS::SERIAL::Getch() {
  if (pserial == NULL || *pserial == 0) {
    int nRandom = 0;
    switch (nRandom) {
    case 0:
      break;
    case 1:
      pserial = "WND.Dump();\n";
      break;
    case 2:
      pserial = "WND.Message(WND::WmKey, KEY::Left);\n";
      break;
    case 3:
      pserial = "MENU.Item=5;\n";
      break;
    }
    return -1;
  }
  return *pserial++;
}

void BIOS::SERIAL::Putch(char ch) {
  char str[2] = {ch, 0};
  fprintf(stderr, "%s", str);
}

const char *BIOS::VER::GetHardwareVersion() {
  return (sizeof(void *) == 8) ? "x64" : "x86";
}

const char *BIOS::VER::GetSystemVersion() { return "WIN32"; }

const char *BIOS::VER::GetFpgaVersion() { return "0.00"; }

const char *BIOS::VER::GetDfuVersion() { return "0.00"; }

ui32 BIOS::VER::GetSerialNumber() { return 0x0006ab0; }

ui32 BIOS::VER::GetDisplayType() { return ToDword('v', 'g', 'a', 0); }

void BIOS::VER::DrawLogo(int x, int y) {}

void BIOS::SYS::SetBacklight(int nLevel) // 0..100
{}

void BIOS::SYS::SetVolume(int nLevel) // 0..100
{}
/*
void BIOS::ADC::ConfigureBuffer(int nLength)
{
        g_nBufferLen = nLength;
}

void BIOS::ADC::GetBufferRange(int& nBegin, int& nEnd)
{
        nBegin = 8;
        nEnd = g_nBufferLen;
}*/

BIOS::ADC::EState BIOS::ADC::GetState() {
  int nAux = 0;
  if (0)
    nAux |= BIOS::ADC::Start;
  if (0)
    nAux |= BIOS::ADC::Empty;
  if (0)
    nAux |= BIOS::ADC::Full;

  return (BIOS::ADC::EState)(nAux);
}

int BIOS::SYS::Get(int, int) { return 0; }

void BIOS::SYS::Set(int, int) {}

int BIOS::ADC::GetPointer() {
  return 2048; // 4096;
}

int BIOS::SYS::GetTemperature() { return 2068; }

int BIOS::SYS::GetCoreVoltage() { return 1024; }

void BIOS::SYS::Standby(bool) {}

const void *BIOS::LCD::GetCharRom() { return (void *)GetFont(0); }

bool BIOS::MOUSE::IsSupported() { return true; }

int BIOS::MOUSE::GetX() { return DEVICE->mousex; }

int BIOS::MOUSE::GetY() { return DEVICE->mousey; }

bool BIOS::MOUSE::GetDown() {
  int nm = DEVICE->moused;
  DEVICE->moused = 0;
  return nm ? true : false;
}

void *BIOS::SYS::IdentifyApplication(int nCode) {
  if (nCode == BIOS::SYS::EApp1)
    return (void *)"DSO_APP: Gabonator's alternative UI;Alter UI;Gabriel Valky "
                   "2013;";
  return NULL;
}

int BIOS::SYS::Execute(int nCode) { return 0; }

ui32 *BIOS::GPIO::GetRegister(int nPort, int nReg) {
  static ui32 dummy;
  return &dummy;
}

void BIOS::GPIO::SetState(int nPort, int nPin, int nState) {}

void BIOS::GPIO::SetPin(int nPort, int nPin, bool bOn) {}

bool BIOS::GPIO::GetPin(int nPort, int nPin) { return false; }

#ifdef _VERSION2

bool BIOS::MEMORY::PageWrite(int nPage, const ui8 *pBuffer) { return true; }

bool BIOS::MEMORY::PageRead(int nPage, ui8 *pBuffer) { return true; }

bool BIOS::MEMORY::PageErase(int nPage) { return true; }

void BIOS::MEMORY::LinearStart() {}

bool BIOS::MEMORY::LinearFinish() { return true; }

bool BIOS::MEMORY::LinearProgram(ui32 nAddress, unsigned char *pData,
                                 int nLength) {
  return true;
}

/*
        class FAT
        {
        public:
                enum EResult
                {
                        EOk,
                        EDiskError,
                        EIntError,
                        ENoFile,
                        ENoPath,
                        EDiskFull
                };

                struct TFindFile
                {
                        ui32 nFileLength;
                        ui16 nDate;
                        ui16 nTime;
                        ui8 nAtrib;
                        char strName[13];
                };

                static EResult Init();
                static EResult Open(const char* strName, ui8 nIoMode);
                static EResult Read(ui8* pSectorData);
                static EResult Write(ui8* pSectorData);
                static EResult Close(int nSize = -1);

                static EResult OpenDir(char* strPath);
                static EResult FindNext(TFindFile* pFile);
        };
*/

BIOS::FAT::EResult BIOS::FAT::Init() { return BIOS::FAT::EOk; }

bool bResetFind = true;
HANDLE hFindFile = NULL;
char strFind[128];

BIOS::FAT::EResult BIOS::FAT::OpenDir(char *strPath) {
  BIOS::UTIL::StrCpy(strFind, strPath);
  char *strFound = NULL;
  while ((strFound = strstr(strFind, "/")) != NULL)
    *strFound = '\\';
  if (strFind[0] != 0)
    BIOS::UTIL::StrCat(strFind, "\\");
  BIOS::UTIL::StrCat(strFind, "*.*");
  bResetFind = true;
  return BIOS::FAT::EOk;
}

BIOS::FAT::EResult BIOS::FAT::FindNext(TFindFile *pFile) {
  WIN32_FIND_DATA FindFileData;

  if (bResetFind) {
    bResetFind = false;
    hFindFile = FindFirstFile(strFind, &FindFileData);
  } else {
    if (!FindNextFile(hFindFile, &FindFileData)) {
      FindClose(hFindFile);
      return BIOS::FAT::ENoFile;
    }
  }

  if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
    pFile->nAtrib = BIOS::FAT::EArchive; // AM_ARC
  else
    pFile->nAtrib = BIOS::FAT::EDirectory; // AM_ARC

  pFile->nFileLength = FindFileData.nFileSizeLow;
  char strName[128] = {0};
  BIOS::UTIL::StrCpy(strName, FindFileData.cFileName);
  strName[12] = 0;
  memcpy(pFile->strName, strName, 13);

  SYSTEMTIME sysTime;
  FileTimeToSystemTime(&FindFileData.ftCreationTime, &sysTime);

  pFile->nDate = ((DWORD)(sysTime.wYear - 1980) << 9) |
                 ((DWORD)sysTime.wMonth << 5) | ((DWORD)sysTime.wDay << 0);
  pFile->nTime = ((DWORD)sysTime.wHour << 11) | ((DWORD)sysTime.wMinute << 5) |
                 ((DWORD)sysTime.wSecond >> 1);

  if (!hFindFile) {
    FindClose(hFindFile);
    return BIOS::FAT::ENoFile;
  }
  return BIOS::FAT::EOk;
}

FILEINFO fatFile;
BIOS::FAT::EResult BIOS::FAT::Open(const char *strName, ui8 nIoMode) {
  if (nIoMode == BIOS::DSK::IoRead)
    fatFile.f = fopen(strName, "rb");
  if (nIoMode == BIOS::DSK::IoWrite)
    fatFile.f = fopen(strName, "wb");
  fatFile.nSectors = 0;
  int e = GetLastError();
  return (fatFile.f != NULL && fatFile.f != INVALID_HANDLE_VALUE)
             ? BIOS::FAT::EOk
             : BIOS::FAT::EIntError;
}

BIOS::FAT::EResult BIOS::FAT::Seek(ui32 lOffset) {
  fseek(fatFile.f, lOffset, SEEK_SET);
  return BIOS::FAT::EOk;
}

BIOS::FAT::EResult BIOS::FAT::Read(ui8 *pSectorData) {
  return BIOS::DSK::Read(&fatFile, pSectorData) ? BIOS::FAT::EOk
                                                : BIOS::FAT::EIntError;
}

BIOS::FAT::EResult BIOS::FAT::Write(ui8 *pSectorData) {
  return BIOS::DSK::Write(&fatFile, pSectorData) ? BIOS::FAT::EOk
                                                 : BIOS::FAT::EIntError;
}

BIOS::FAT::EResult BIOS::FAT::Close(int nSize /*= -1*/) {
  return BIOS::DSK::Close(&fatFile, nSize) ? BIOS::FAT::EOk
                                           : BIOS::FAT::EIntError;
}

ui32 BIOS::FAT::GetFileSize() {
  fseek(fatFile.f, 0, SEEK_END);
  // get the file size
  int nSize = ftell(fatFile.f);
  rewind(fatFile.f);
  return nSize;
}
#endif

bool BIOS::SYS::IsColdBoot() { return true; }

char *BIOS::SYS::GetSharedBuffer() {
  static char buf[4096];
  return buf;
}

int BIOS::SYS::GetSharedLength() { return 4096; }

void NullFunction() {}

void *BIOS::SYS::GetProcAddress(const char *strFuncName) {
#define EXPORT(f, decl)                                                        \
  if (strcmp(strFuncName, #f) == 0)                                            \
    return (void *)(decl)&f;
#define EXPORT_ALIAS(al, f, decl)                                              \
  if (strcmp(strFuncName, #al) == 0)                                           \
    return (void *)(decl)&f;
  EXPORT(BIOS::LCD::PutPixel, void (*)(int, int, ui16));
  EXPORT(BIOS::LCD::Print, int (*)(int, int, ui16, ui16, const char *));
  EXPORT(BIOS::KEY::GetKeys, ui16(*)());
  EXPORT(BIOS::SYS::Execute, void (*)(int));
  EXPORT(BIOS::LCD::Printf,
         int (*)(int x, int y, unsigned short clrf, unsigned short clrb,
                 const char *format, ...));

  EXPORT_ALIAS(PutPixel, BIOS::LCD::PutPixel, void (*)(int, int, ui16));
  EXPORT_ALIAS(Print, BIOS::LCD::Print,
               int (*)(int, int, ui16, ui16, const char *));
  EXPORT_ALIAS(GetKeys, BIOS::KEY::GetKeys, ui16(*)());
  EXPORT_ALIAS(Execute, BIOS::SYS::Execute, void (*)(int));
  EXPORT_ALIAS(Printf, BIOS::LCD::Printf,
               int (*)(int x, int y, unsigned short clrf, unsigned short clrb,
                       const char *format, ...));

  EXPORT_ALIAS(gBiosInit, NullFunction, void (*)());
  EXPORT_ALIAS(gBiosExit, NullFunction, void (*)());

#undef EXPORT
  return 0;
}
