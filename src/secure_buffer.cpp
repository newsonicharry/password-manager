#include "secure_buffer.h"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <sodium.h>
#include <sodium/utils.h>
#include <iostream>


SecureBuffer::SecureBuffer(std::size_t length)
{
  buffer_.resize(length);
  
  sodium_mlock(buffer_.data(), length);
}


SecureBuffer::~SecureBuffer() noexcept
{
  if (buffer_.capacity() == 0)
  {
    return;
  }

  sodium_munlock(buffer_.data(), buffer_.capacity());
  sodium_memzero(buffer_.data(), buffer_.capacity());
}


auto SecureBuffer::operator=(SecureBuffer&& other) noexcept -> SecureBuffer&
{
  if (this == &other)
  {
    return *this;  
  }

  sodium_memzero(buffer_.data(), buffer_.capacity());
  buffer_ = std::move(other.buffer_);
    
  return *this;
}



auto SecureBuffer::operator[](std::size_t index) const -> const std::byte&
{
  assert(buffer_.capacity() > 0 && "No data in secure buffer");
  return buffer_[index];
}


auto SecureBuffer::get_write_ptr() -> std::byte*
{
  // can only write to it if a length has been initalized and theres no data in it already
  // if (buffer_.capacity() == 0 || buffer_.size() != 0){
    // return nullptr;
  // }

  return buffer_.data();
}


