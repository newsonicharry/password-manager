#include "secure_buffer.h"
#include <cstddef>
#include <cstring>
#include <sodium.h>
#include <sodium/utils.h>


SecureBuffer::SecureBuffer(unsigned long long length)
: length_{length}
, has_data_{false}
{
  buffer_ = new std::byte[length_];
  // ensures data isnt swapped to disk
  sodium_mlock(buffer_, length_);
}


SecureBuffer::~SecureBuffer()
{
  if (buffer_ == nullptr){
    return;
  }
  
  sodium_memzero(buffer_, length_);
  delete[] buffer_;
}


SecureBuffer::SecureBuffer(const SecureBuffer& secure_buffer)
: length_{secure_buffer.length_}
, has_data_{secure_buffer.has_data_}
{
  this->~SecureBuffer();

  buffer_ = new std::byte[secure_buffer.length_];  
}


SecureBuffer::SecureBuffer(SecureBuffer&& secure_buffer) noexcept
: buffer_{secure_buffer.buffer_}
, length_{secure_buffer.length_}
, has_data_{secure_buffer.has_data_}
{
  secure_buffer.buffer_ = nullptr;  
  secure_buffer.length_ = 0;
  secure_buffer.has_data_ = false;
}


auto SecureBuffer::operator=(SecureBuffer&& secure_buffer) noexcept -> SecureBuffer&
{
  if (this == &secure_buffer){
    return *this;
  }

  this->~SecureBuffer();

  buffer_ = secure_buffer.buffer_;
  length_ = secure_buffer.length_;
  has_data_ = secure_buffer.has_data_;
 
  secure_buffer.buffer_ = nullptr;
  secure_buffer.length_ = 0;
  secure_buffer.has_data_ = false;

  return *this;
}


auto SecureBuffer::operator=(const SecureBuffer& secure_buffer) -> SecureBuffer&
{
  if (this == &secure_buffer){
    return *this;
  }
  
  sodium_memzero(buffer_, length_);
  delete[] buffer_;
  
  buffer_ = new std::byte[secure_buffer.length_];
  length_ = secure_buffer.length_;
  has_data_ = secure_buffer.has_data_;

  std::memcpy(buffer_, secure_buffer.buffer_, length_);

  return *this;
}


auto SecureBuffer::operator[](std::size_t index) -> const std::byte&
{ 
  assert(index < length_ && "Index not within bounds of array");
  assert(has_data_ && "Buffer has not been filled with anything");
  return buffer_[index];
}


auto SecureBuffer::get_write_ptr() -> std::byte*
{
  if (has_data_){
    return nullptr;
  }

  return buffer_;
}
