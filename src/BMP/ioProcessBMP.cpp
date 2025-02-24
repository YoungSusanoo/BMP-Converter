#include "ioProcessBMP.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <string>

namespace bmpconv
{
  constexpr size_t rgbquad_bits = 4;
  constexpr size_t alignNum = 4;

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
      unsigned char f = reinterpret_cast< unsigned char& >(a);
      return p.at(f);
    };
    auto func = std::bind(lambda, std::placeholders::_1, std::cref(palette));
    std::vector< RGBQUAD > completedColors(rawColors.size());
    std::transform(rawColors.cbegin(), rawColors.cend(), completedColors.begin(), func);
    return completedColors;
  }
}

bmpconv::BMP bmpconv::readBMP(const std::wstring& filename)
{
  std::ifstream in((filename.c_str()), std::ios_base::binary);
  if (!in)
  {
    throw std::invalid_argument("Can't open file");
  }

  BITMAPFILEHEADER bFH;
  BITMAPINFOHEADER bIH;
  in.read(reinterpret_cast< char* >(&bFH), sizeof(bFH));
  in.read(reinterpret_cast< char* >(&bIH), sizeof(bIH));

  size_t bytesToSkip = bIH.biSize - sizeof(bIH);
  auto fileIt = std::istreambuf_iterator< char >(in);
  std::advance(fileIt, bytesToSkip);

  size_t usedColors = bIH.biClrUsed ? bIH.biClrUsed : 1 << bIH.biBitCount;
  std::vector< char > rawPalette(usedColors * rgbquad_bits);
  std::copy_n(fileIt, usedColors * rgbquad_bits, rawPalette.begin());
  std::vector< RGBQUAD > palette = paletteToRGBQUAD(rawPalette);

  size_t currPos = sizeof(bFH) + bIH.biSize + usedColors * rgbquad_bits;
  if (currPos < bFH.bfOffBits)
  {
    std::advance(fileIt, bFH.bfOffBits - currPos);
  }

  size_t colorBytes = bIH.biWidth * bIH.biBitCount / 8;
  size_t amountFromDivAlign = colorBytes % alignNum;
  std::vector< char > colors(bIH.biHeight * colorBytes);
  auto colorsIt = colors.begin();
  for (size_t i = 0; i < bIH.biHeight; i++)
  {
    std::copy_n(fileIt, colorBytes, colorsIt);
    size_t remaining = (alignNum - amountFromDivAlign) % alignNum;
    std::advance(fileIt, remaining);
    std::advance(colorsIt, colorBytes);
  }
  std::vector< RGBQUAD > rgbQuadColors = getFrom8(colors, palette);
  return BMP(rgbQuadColors, bIH.biWidth, bIH.biHeight);
}

void bmpconv::writeBMP(const std::wstring& filename, const BMP& bmp)
{
  BITMAPFILEHEADER bFH;
  BITMAPINFOHEADER bIH;

  size_t alignMultiply = ((alignNum - 1) + bmp.getWidth()) / alignNum;
  size_t bytesToAlign = alignNum * alignMultiply - bmp.getWidth();
  bFH.bfType = 0x4D42;
  bFH.bfOffBits = sizeof(bIH) + sizeof(bFH);
  bFH.bfSize = sizeof(bIH) + sizeof(bFH);
  bFH.bfSize += (bmp.getWidth() + bytesToAlign) * bmp.getHeight() * 3;

  bIH.biSize = sizeof(bIH);
  bIH.biWidth = bmp.getWidth();
  bIH.biHeight = bmp.getHeight();
  bIH.biPlanes = 1;
  bIH.biBitCount = 24;
  bIH.biCompression = 0;
  bIH.biSizeImage = 0;
  bIH.biXPelsPerMeter = 0;
  bIH.biYPelsPerMeter = 0;
  bIH.biClrUsed = 0;
  bIH.biClrImportant = 0;
  std::ofstream file(filename.c_str(), std::ios::binary);
  file.write(reinterpret_cast< char* >(&bFH), sizeof(bFH));
  file.write(reinterpret_cast< char* >(&bIH), sizeof(bIH));

  auto colorsIt = bmp.getColors().cbegin();
  auto outputLambda = [](RGBQUAD color, std::ofstream& file)
  {
    size_t size = sizeof(color.rgbBlue);
    file.write(reinterpret_cast< char* >(&color.rgbBlue), size);
    file.write(reinterpret_cast< char* >(&color.rgbGreen), size);
    file.write(reinterpret_cast< char* >(&color.rgbRed), size);
  };
  constexpr char charToWrite = '0';
  auto func = std::bind(outputLambda, std::placeholders::_1, std::ref(file));
  for (size_t i = 0; i < bmp.getHeight(); i++)
  {
    std::for_each(colorsIt, colorsIt + bmp.getWidth(), func);
    file.write(&charToWrite, bytesToAlign);
    std::advance(colorsIt, bmp.getWidth());
  }
}
