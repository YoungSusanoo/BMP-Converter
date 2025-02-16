#include "WinApiHandle.hpp"

#include <utility>

namespace dowsorter
{
  bool isValid(HANDLE handle)
  {
    return (handle != NULL) && (handle != INVALID_HANDLE_VALUE);
  }
}

dowsorter::WinApiHandle::WinApiHandle(WinApiHandle&& rhs) noexcept:
  handle_{ std::exchange(rhs.handle_, reinterpret_cast< HANDLE >(NULL)) }
{}

dowsorter::WinApiHandle::WinApiHandle(HANDLE handle) noexcept:
  handle_{ handle }
{}

dowsorter::WinApiHandle::~WinApiHandle()
{
  close();
}

dowsorter::WinApiHandle& dowsorter::WinApiHandle::operator=(WinApiHandle&& rhs) noexcept
{
  if (this != std::addressof(rhs))
  {
    close();
    handle_ = std::exchange(rhs.handle_, reinterpret_cast< HANDLE >(NULL));
  }
  return *this;
}

dowsorter::WinApiHandle::operator bool() const noexcept
{
  return isValid(handle_);
}

dowsorter::WinApiHandle::operator HANDLE() const noexcept
{
  return handle_;
}

dowsorter::WinApiHandle::operator DWORD_PTR() const noexcept
{
  return reinterpret_cast< DWORD_PTR >(handle_);
}

HANDLE* dowsorter::WinApiHandle::operator&() noexcept
{
  close();
  return &handle_;
}

void dowsorter::WinApiHandle::close() noexcept
{
  if (!isValid(handle_))
  {
    CloseHandle(handle_);
    handle_ = NULL;
  }
}