#ifndef IOPROCESSBMP_HPP
#define IOPROCESSBMP_HPP
#include <iosfwd>

#include "BMP.hpp"

namespace bmpconv
{
  BMP readBMP(const std::wstring& filename);
  void writeBMP(const std::wstring& filename, const BMP& bmp);
}

#endif
