#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

class Exception : public std::runtime_error
{
public:
  enum class ExceptionType : uint8_t
  {
    HashingError,
    FileError,
    DecryptionError,
    BitwardenConversionError,
    ParsingError,
    VaultError,
    EncryptionError,
    UiError,
    Size,
  };

  static constexpr std::array<std::string_view, static_cast<std::size_t>(ExceptionType::Size)> TYPE_TO_STRING
  {
    "HASHING ERROR",
    "FILE ERROR",
    "DECRYPTION ERROR",
    "BITWARDEN CONVERSION ERROR",
    "PARSING ERROR",
    "VAULT ERROR",
    "ENCRYPTION ERROR",
    "UI ERROR",
  };

  Exception(const std::string& error_msg, ExceptionType error_type)
  : std::runtime_error(error_msg)
  , exception_type_{error_type}
  {}  
  
  [[nodiscard]]
  auto type() const -> ExceptionType { return exception_type_; }

private:
  ExceptionType exception_type_;
  
};

