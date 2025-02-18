#include "BMP.hpp"

bmpconv::BMP::BMP(const BMP& rhs):
  colors_{ rhs.colors_ },
  width_{ rhs.width_ },
  height_{ rhs.height_ }
{}

bmpconv::BMP::BMP(BMP&& rhs) noexcept:
  BMP{}
{
  colors_.swap(rhs.colors_);
  width_ = rhs.width_;
  height_ = rhs.height_;
}

bmpconv::BMP::BMP(const std::vector< RGBQUAD >& colors, size_t width, size_t height):
  colors_{ colors },
  width_{ width },
  height_{ height }
{}

const std::vector< RGBQUAD >& bmpconv::BMP::getColors() const
{
  return colors_;
}

size_t bmpconv::BMP::getWidth() const
{
  return width_;
}

size_t bmpconv::BMP::getHeight() const
{
  return height_;
}
