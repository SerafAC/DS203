#include <Source/HwLayer/Bios.h>
#include <Source/HwLayer/Types.h>

// Non-size_t BIOS::UTIL::StrLen, because the code assumes BIOS::UTIL::StrLen()
// returns something compatible with int
int BIOS::UTIL::StrLen(const char *s) {
  const char *t = s;
  while (*t) {
    t++;
  }
  return (int)(t - s); // assuming in int range
}

char *BIOS::UTIL::StrCpy(char *dst, const char *src) {
  char *t = dst;
  while ((*dst++ = *src++)) {
  }
  return t;
}

char *BIOS::UTIL::StrCat(char *dst, const char *src) {
  StrCpy(dst + BIOS::UTIL::StrLen(dst), src);
  return dst;
}
