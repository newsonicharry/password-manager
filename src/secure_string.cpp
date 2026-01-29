#include "secure_string.h"
#include <string>
#include <string_view>

SecureString::SecureString(std::string_view secure_str)
: str_{secure_str}
{}


std::string_view SecureString::get() const{
  return str_;
}
