#pragma once

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
public:
  enum class ExceptionType
  {
    HashingError,
    FileError,
    DecryptionError,
    BitwardenConversionError,
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

