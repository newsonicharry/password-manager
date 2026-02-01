#pragma once

#include <cassert>
class SecureBuffer{
public:
  SecureBuffer(unsigned long long length);

  ~SecureBuffer();

  SecureBuffer(SecureBuffer&) = delete;

  SecureBuffer(SecureBuffer&&) noexcept;

  auto operator=(SecureBuffer&&) noexcept -> SecureBuffer&;

  [[nodiscard]]
  auto get_length() const -> unsigned long long { return length_; }

  auto operator[](int index) -> const unsigned char&;
    
private:
  unsigned char* buffer_;
  unsigned long long length_;
};

