#ifndef BMP_HPP
#define BMP_HPP

#include <vector>
#include <cstddef>
#include <windows.h>
#include <wingdi.h>

namespace bmpconv
{
  class BMP
  {
  public:
    BMP() = default;
    BMP(const BMP& rhs);
    BMP(BMP&& rhs) noexcept;
    BMP(const std::vector< RGBQUAD >& colors, size_t width, size_t height);

    const std::vector< RGBQUAD >& getColors() const;
    size_t getWidth() const;
    size_t getHeight() const;

  private:
    std::vector< RGBQUAD > colors_;
    size_t width_;
    size_t height_;
  };
}

#endif
