#pragma once

#include <cassert>
#include <cstddef>
#include <vector>



class SecureBuffer{
public:
  SecureBuffer() = default;
  
  SecureBuffer(std::size_t length);

  ~SecureBuffer() noexcept;

  SecureBuffer(const SecureBuffer& other) = delete;

  SecureBuffer(SecureBuffer&& other) noexcept = default;

  auto operator=(const SecureBuffer& other) -> SecureBuffer& = delete;

  auto operator=(SecureBuffer&& other) noexcept -> SecureBuffer&;

  // useful for writing using a pointer but the .size() doesnt actually change
  void resize_to_capacity() {buffer_.resize(buffer_.capacity());}
  
  [[nodiscard]]
  auto size() const -> unsigned long long { return buffer_.size(); }

  [[nodiscard]]
  auto get_write_ptr() -> std::byte*;

  [[nodiscard]]
  auto get_read_ptr() const -> const std::byte* { return buffer_.data(); }

  [[nodiscard]]
  auto begin() const -> const std::byte* { return buffer_.data(); }

  [[nodiscard]]
  auto end() const -> const std::byte* { return buffer_.data()+size(); }
    
  auto operator[](std::size_t index) const -> const std::byte&;

    
private:
  std::vector<std::byte> buffer_{};
};

