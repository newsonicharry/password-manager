#pragma once

#include <cassert>
#include <cstddef>

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
  auto get_write_ptr() -> std::byte*;

  [[nodiscard]]
  auto get_read_ptr() -> const std::byte* { return buffer_; }
  
  auto operator[](std::size_t index) -> const std::byte&;
    
private:
  std::byte* buffer_;
  unsigned long long length_;
  bool has_data_;
};

