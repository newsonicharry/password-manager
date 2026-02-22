#pragma once

#include <cassert>
#include <cstddef>
#include <vector>


class SecureBuffer{
public:
  SecureBuffer() = default;
  
  SecureBuffer(unsigned long long length);

  ~SecureBuffer() noexcept;

  SecureBuffer(const SecureBuffer& other) = delete;

  SecureBuffer(SecureBuffer&& other) noexcept = default;

  auto operator=(const SecureBuffer& other) -> SecureBuffer& = delete;

  auto operator=(SecureBuffer&& other) noexcept -> SecureBuffer& = default;
  
  void set_length();

  [[nodiscard]]
  auto size() const -> unsigned long long { return buffer_.capacity(); }

  [[nodiscard]]
  auto get_write_ptr() -> std::byte*;

  [[nodiscard]]
  auto get_read_ptr() const -> const std::byte* { return buffer_.data(); }
  
  auto operator[](std::size_t index) -> const std::byte&;
    
private:
  std::vector<std::byte> buffer_{};
  bool has_data_{};
};

