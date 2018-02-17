#pragma once
#ifndef DSO_CORE_SERIALIZE_H
#define DSO_CORE_SERIALIZE_H

#include <Source/Core/Utils.h>
#include <Source/HwLayer/Bios.h>
#include <Source/HwLayer/Types.h>
class CStream;

#define _E(x) reinterpret_cast<NATIVEENUM *>(&x)

class CSerialize {
public:
  virtual CSerialize &operator<<(CStream &stream);
  virtual CSerialize &operator>>(CStream &stream);
};

class CStream {
  PVOID m_pBuffer;
  int m_nOffset;
  int m_nLength;

public:
  CStream(PVOID pBuffer, int nLength) {
    m_pBuffer = pBuffer;
    m_nLength = nLength;
    m_nOffset = 0;
  }
  CStream(PSTR strBuffer) {
    m_pBuffer = strBuffer;
    m_nLength = CUtils::StrLen(strBuffer);
    m_nOffset = 0;
  }

  void Reset() { m_nOffset = 0; }

  CStream(NATIVEENUM *e)
      : m_pBuffer(e), m_nOffset(0), m_nLength(sizeof(NATIVEENUM)) {}
  template <class T>
  CStream(T &n) : m_pBuffer(&n), m_nOffset(0), m_nLength(sizeof(T)) {}

private:
  template <class T> CStream(T *n);

public:
  CStream &operator<<(const CStream &data) {
    _ASSERT(data.m_nLength < m_nLength - m_nOffset);
    memcpy(((ui8 *)m_pBuffer) + m_nOffset, data.m_pBuffer, data.m_nLength);
    m_nOffset += narrow_cast<unsigned>(data.m_nLength);
    return *this;
  }
  CStream &operator>>(const CStream &data) {
    memcpy(data.m_pBuffer, ((ui8 *)m_pBuffer) + m_nOffset, data.m_nLength);
    m_nOffset += narrow_cast<unsigned>(data.m_nLength);
    return *this;
  }
  //
  CStream &operator<<(CSerialize &stream) {
    stream << *this;
    return *this;
  }

  CStream &operator>>(CSerialize &stream) {
    stream >> *this;
    return *this;
  }
  const int &GetLength() { return m_nLength; }

  ui32 GetChecksum() {
    ui32 nAux = 0;
    ui8 *pBuffer = (ui8 *)m_pBuffer;
    for (int i = 0; i < m_nLength; i++, pBuffer++)
      nAux ^= (*pBuffer) << ((i & 3) << 3);
    return nAux;
  }
  ui8 &operator[](int nIndex) {
    _ASSERT(nIndex >= 0 && nIndex < GetLength());
    return ((ui8 *)m_pBuffer)[nIndex];
  }
};

#endif
