#pragma once

#include <filesystem>
#include <string_view>
#include "secure_buffer.h"

namespace fs = std::filesystem;

class FileManager{
public:
  FileManager() = default;

  auto does_directory_exist() -> bool;
  void create_directory();  

  auto does_user_exist(std::string_view username) -> bool;

  void decrypt_to_secure_buffer(SecureBuffer& secure_buffer, std::string_view username);

  [[nodiscard]]
  auto get_path() -> fs::path{ return directory_path_; };  
  
private:


  static auto get_directory_path() -> fs::path;

  fs::path directory_path_{get_directory_path()};


  
};


