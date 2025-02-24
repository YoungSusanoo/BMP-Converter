#include "WinApiHandle.hpp"

#include <utility>

namespace bmpconv
{
  bool isValid(HANDLE handle)
  {
    return (handle != NULL) && (handle != INVALID_HANDLE_VALUE);
  }
}

bmpconv::WinApiHandle::WinApiHandle(WinApiHandle&& rhs) noexcept:
  handle_{ std::exchange(rhs.handle_, reinterpret_cast< HANDLE >(NULL)) }
{}

bmpconv::WinApiHandle::WinApiHandle(HANDLE handle) noexcept:
  handle_{ handle }
{}

bmpconv::WinApiHandle::~WinApiHandle()
{
  close();
}

bmpconv::WinApiHandle& bmpconv::WinApiHandle::operator=(WinApiHandle&& rhs) noexcept
{
  if (this != std::addressof(rhs))
  {
    close();
    handle_ = std::exchange(rhs.handle_, reinterpret_cast< HANDLE >(NULL));
  }
  return *this;
}

bmpconv::WinApiHandle::operator bool() const noexcept
{
  return isValid(handle_);
}

bmpconv::WinApiHandle::operator HANDLE() const noexcept
{
  return handle_;
}

bmpconv::WinApiHandle::operator DWORD_PTR() const noexcept
{
  return reinterpret_cast< DWORD_PTR >(handle_);
}

HANDLE* bmpconv::WinApiHandle::operator&() noexcept
{
  close();
  return &handle_;
}

void bmpconv::WinApiHandle::close() noexcept
{
  if (!isValid(handle_))
  {
    CloseHandle(handle_);
    handle_ = NULL;
  }
}