#pragma once

#include <cassert>
class SecureBuffer{
public:
  SecureBuffer(unsigned long long length);

  ~SecureBuffer() noexcept;

  SecureBuffer(const SecureBuffer&);

  SecureBuffer(SecureBuffer&&) noexcept;

  auto operator=(SecureBuffer&&) noexcept -> SecureBuffer&;

  auto operator=(const SecureBuffer&) -> SecureBuffer&;

  [[nodiscard]]
  auto get_length() const -> unsigned long long { return length_; }

  [[nodiscard]]
  auto get_empty_buffer_ptr() -> unsigned char*;
  
  auto operator[](int index) -> const unsigned char&;
    
private:
  unsigned char* buffer_;
  unsigned long long length_;
  bool has_data_;
};

