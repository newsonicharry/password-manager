#include <fstream>
#include "converter.h"
#include "exception.h"

namespace
{
  
}// unnamed namespace


auto convert_from_bitwarden_json(const fs::path& json_path) -> SecureBuffer
{
  if (json_path.extension() != "json")
  {
    throw Exception("File given is not a json.\n", Exception::ExceptionType::BitwardenConversionError);
  }

  std::ifstream file{json_path};

  if (!file.is_open())
  {
    throw Exception("Failed to open bitwarden json.\n", Exception::ExceptionType::BitwardenConversionError);
  }

  
  
  SecureBuffer buffer{};
  return buffer;
}
