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
 
  auto get_user_path(std::string_view username) -> fs::path;
  auto does_user_exist(std::string_view username) -> bool;
  void delete_user(std::string_view username);

  void decrypt_to_secure_buffer(SecureBuffer& secure_buffer, std::string_view username);

  [[nodiscard]]
  auto get_directory_path() -> fs::path{ return directory_path_; };  
  
private:  
  static auto find_directory_path() -> fs::path;

  fs::path directory_path_{find_directory_path()};


  
};


