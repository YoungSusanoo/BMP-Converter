#include "ioProcessBMP.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <string>

namespace bmpconv
{
  std::vector< RGBQUAD > paletteToRGBQUAD(const std::vector< char >& rawPalette)
  {
    std::vector< RGBQUAD > rgbVec(rawPalette.size() / 4);
    for (size_t i = 0; i < rgbVec.size(); i++)
    {
      rgbVec[i].rgbBlue = rawPalette.at(i * 4);
      rgbVec[i].rgbGreen = rawPalette.at(i * 4 + 1);
      rgbVec[i].rgbRed = rawPalette.at(i * 4 + 2);
      rgbVec[i].rgbReserved = rawPalette.at(i * 4 + 3);
    }
    return rgbVec;
  }

  std::vector< RGBQUAD > getFrom8(const std::vector< char >& rawColors, const std::vector< RGBQUAD >& palette)
  {
    auto lambda = [](char a, const std::vector< RGBQUAD >& p) -> RGBQUAD
    {
      return p.at(reinterpret_cast< unsigned char& >(a));
    };
    auto func = std::bind(lambda, std::placeholders::_1, std::cref(palette));
    std::vector< RGBQUAD > completedColors(rawColors.size());
    std::transform(rawColors.cbegin(), rawColors.cend(), completedColors.begin(), func);
    return completedColors;
  }
}

bmpconv::BMP bmpconv::readBMP(const std::wstring& filename)
{
  constexpr size_t rgbquad_bits = 4;
  constexpr size_t alignNum = 4;
  std::ifstream in((filename.c_str()), std::ios_base::binary);
  if (!in)
  {
    throw std::invalid_argument("Can't open file");
  }

  BITMAPFILEHEADER bFH;
  BITMAPINFOHEADER bIH;
  in.read(reinterpret_cast< std::istream::char_type* >(&bFH), sizeof(bFH));
  in.read(reinterpret_cast< std::istream::char_type* >(&bIH), sizeof(bIH));

  size_t bytesToSkip = bIH.biSize - sizeof(bIH);
  auto fileIt = std::istreambuf_iterator< char >(in);
  std::advance(fileIt, bytesToSkip);

  size_t usedColors = bIH.biClrUsed ? bIH.biClrUsed : 1 << bIH.biBitCount;
  std::vector< char > rawPalette(usedColors * rgbquad_bits);
  std::copy_n(fileIt, bIH.biClrUsed, rawPalette.begin());
  std::vector< RGBQUAD > palette = paletteToRGBQUAD(rawPalette);

  size_t currPos = sizeof(bFH) + bIH.biSize + usedColors * rgbquad_bits;
  if (currPos < bFH.bfOffBits)
  {
    std::advance(fileIt, bFH.bfOffBits - currPos);
  }

  std::vector< char > colors(bIH.biHeight * bIH.biWidth * bIH.biBitCount / 8);
  auto colorsIt = colors.begin();
  for (size_t i = 0; i < bIH.biHeight; i++)
  {
    std::copy_n(fileIt, bIH.biWidth * bIH.biBitCount, colorsIt);
    size_t remaining = alignNum - bIH.biWidth * bIH.biBitCount % alignNum;
    std::advance(fileIt, remaining);
  }
  std::vector< RGBQUAD > rgbQuadColors = getFrom8(colors, palette);
  return BMP(rgbQuadColors, bIH.biWidth, bIH.biHeight);
}
