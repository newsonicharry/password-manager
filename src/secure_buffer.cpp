#include "secure_buffer.h"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <sodium.h>
#include <sodium/utils.h>


SecureBuffer::SecureBuffer(unsigned long long length)
{
  buffer_.reserve(length);
  sodium_mlock(buffer_.data(), length);
}


SecureBuffer::~SecureBuffer() noexcept
{
  if (buffer_.size() == 0){
    return;
  }
  
  sodium_memzero(buffer_.data(), buffer_.size());
}


auto SecureBuffer::operator[](std::size_t index) -> const std::byte&
{
  assert(buffer_.capacity() > 0 && "No data in secure buffer");
  return buffer_[index];
}


auto SecureBuffer::get_write_ptr() -> std::byte*
{
  if (buffer_.capacity() == 0){
    return nullptr;
  }

  return buffer_.data();
}
