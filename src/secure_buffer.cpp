#include "secure_buffer.h"
#include <cstring>
#include <sodium.h>
#include <sodium/utils.h>

SecureBuffer::SecureBuffer(unsigned long long length)
: length_{length}
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

SecureBuffer::SecureBuffer(SecureBuffer&& secure_buffer) noexcept
: buffer_{secure_buffer.buffer_}
, length_{secure_buffer.length_}
{
  secure_buffer.buffer_ = nullptr;  
}

auto SecureBuffer::operator[](int index) -> const unsigned char&{ 
  assert(index < length_ && index >= 0 && "Index not within bounds of array");
  return buffer_[index];
}

auto SecureBuffer::operator=(SecureBuffer&& secure_buffer) noexcept -> SecureBuffer&{
  if (this != &secure_buffer){

    delete[] buffer_;
    
    buffer_ = secure_buffer.buffer_;
    length_ = secure_buffer.length_;

    secure_buffer.buffer_ = nullptr;
    secure_buffer.length_ = 0; 
  }

  return *this;
}
