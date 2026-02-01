#pragma once

#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

class FileManager{
public:
  FileManager() = default;

  auto does_directory_exist() -> bool;
  void create_directory();  
  auto does_user_exist(std::string_view username) -> bool;

  
  
private:
  fs::path directory_path_{get_directory_path()};

  static auto get_directory_path() -> fs::path;

  
};


