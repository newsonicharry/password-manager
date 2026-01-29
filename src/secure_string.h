#ifndef SECURE_STRING_H
#define SECURE_STRING_H

#include <string>
#include <string_view>


/**
* @class SecureString
* @brief holds sensitive strings that can't be leaked
*
* Example Usage:
* SecureString password_container{"password123"};
* std::string_view password{SecureString.get()};
*/
class SecureString{
public:

  SecureString() = delete;
  SecureString(std::string_view secure_str);

  ~SecureString();

  std::string_view get() const;

  
private:
  const std::string str_;
};

#endif
