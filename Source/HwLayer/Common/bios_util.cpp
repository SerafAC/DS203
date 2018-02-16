#include <Source/HwLayer/Bios.h>
#include <Source/HwLayer/Types.h>
#if false
// Non-size_t CUtils::StrLen, because the code assumes CUtils::StrLen()
// returns something compatible with int
int CUtils::StrLen(const char *s) {
  const char *t = s;
  while (*t) {
    t++;
  }
  return (int)(t - s); // assuming in int range
}

char *CUtils::StrCpy(char *dst, const char *src) {
  char *t = dst;
  while ((*dst++ = *src++)) {
  }
  return t;
}

char *CUtils::StrCat(char *dst, const char *src) {
  StrCpy(dst + CUtils::StrLen(dst), src);
  return dst;
}

int CUtils::StrCmp(const char *a, const char *b) {
  while (*a != '\0' && *a == *b) {
    a++;
    b++;
  }
  return (*(unsigned char *)a) - (*(unsigned char *)b);
}
#endif
