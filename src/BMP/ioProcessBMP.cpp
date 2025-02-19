#include "ioProcessBMP.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>

namespace bmpconv
{}

bmpconv::BMP bmpconv::readBMP(const std::wstring& filename)
{
  std::ifstream in((filename.c_str()), std::ios_base::binary);
  if (!in)
  {
    throw std::invalid_argument("Can't open file");
  }

  BITMAPFILEHEADER bmFileHeader;
  BITMAPINFOHEADER bmInfoHeader;
  in.read(reinterpret_cast< std::istream::char_type* >(&bmFileHeader), sizeof(bmFileHeader));
  in.read(reinterpret_cast< std::istream::char_type* >(&bmInfoHeader), sizeof(bmFileHeader));

  size_t bytesToSkip = bmInfoHeader.biSize - sizeof(bmInfoHeader);
  auto it = std::istreambuf_iterator< char >(in);
  std::advance(it, bytesToSkip);

  std::vector< char > palette(bmInfoHeader.biClrUsed);
  std::copy_n(it, bmInfoHeader.biClrUsed, palette.begin());

  size_t currPos = sizeof(bmFileHeader) + bmInfoHeader.biSize + bmInfoHeader.biClrUsed;
  if (currPos < bmFileHeader.bfOffBits)
  {
    std::advance(it, bmFileHeader.bfOffBits - currPos);
  }

}
