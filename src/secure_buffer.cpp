#include "secure_buffer.h"
#include <cstring>
#include <sodium.h>
#include <sodium/utils.h>


SecureBuffer::SecureBuffer(unsigned long long length)
: length_{length}
, has_data_{false}
{
  buffer_ = new unsigned char[length_];
}


SecureBuffer::~SecureBuffer(){
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
  sodium_memzero(buffer_, length_);
  delete[] buffer_;

  buffer_ = new unsigned char[secure_buffer.length_];  
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


auto SecureBuffer::operator=(SecureBuffer&& secure_buffer) noexcept -> SecureBuffer&{
  if (this == &secure_buffer){
    return *this;
  }
 
  sodium_memzero(buffer_, length_);
  delete[] buffer_;

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
  
  buffer_ = new unsigned char[secure_buffer.length_];
  length_ = secure_buffer.length_;
  has_data_ = secure_buffer.has_data_;

  std::memcpy(buffer_, secure_buffer.buffer_, length_);

  return *this;
}


auto SecureBuffer::operator[](int index) -> const unsigned char&{ 
  assert(index < length_ && index >= 0 && "Index not within bounds of array");
  assert(has_data_ && "Buffer has not been filled with anything");
  return buffer_[index];
}


auto SecureBuffer::get_empty_buffer_ptr() -> unsigned char*{
  if (has_data_){
    return nullptr;
  }

  return buffer_;
}

