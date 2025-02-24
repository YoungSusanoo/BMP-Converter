#ifndef WINAPIHANDLE_HPP
#define WINAPIHANDLE_HPP

#include <handleapi.h>

namespace bmpconv
{
  class WinApiHandle
  {
  public:
    WinApiHandle() = default;
    WinApiHandle(const WinApiHandle&) = delete;
    WinApiHandle(WinApiHandle&&) noexcept;
    explicit WinApiHandle(HANDLE) noexcept;
    ~WinApiHandle();

    WinApiHandle& operator=(const WinApiHandle&) = delete;
    WinApiHandle& operator=(WinApiHandle&&) noexcept;

    explicit operator bool() const noexcept;
    operator HANDLE() const noexcept;
    operator DWORD_PTR() const noexcept;
    HANDLE* operator&() noexcept;

    void close() noexcept;

  private:
    HANDLE handle_{ NULL };
  };
}

#endif