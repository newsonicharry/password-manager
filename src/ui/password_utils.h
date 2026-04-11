#pragma once

#include "../secure_buffer.h"
#include <cstdint>
#include <string_view>
#include <bitset>

namespace ui::password_utils
{

  enum class PasswordStrength : std::uint8_t
  {
    VeryWeak = 0,
    Weak,
    Fair,
    Strong,
    VeryStrong,
    Size
  };


  auto classify_password_strength(const std::string& password) -> PasswordStrength;

  
  auto generate_random_password(std::size_t length, std::bitset<4> options) -> SecureBuffer; 
}
