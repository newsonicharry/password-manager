#pragma once


#include <cstddef>
#include <string_view>

namespace constants{
  inline constexpr std::string_view APP_NAME{"password_manager"}; 
  inline constexpr std::size_t NUM_SALT_BYTES{32};  
}

