#pragma once

#include <string_view>

namespace ui::password_strength
{

  enum class PasswordStrength
  {
    VeryWeak = 0,
    Weak,
    Fair,
    Strong,
    VeryStrong,
    Size
  };


  // helpers

  // TODO: change to a secure buffer later
  auto classify_password_strength(std::string_view password) -> PasswordStrength;
 
}
