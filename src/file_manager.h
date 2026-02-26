#pragma once

#include <cstddef>
#include <filesystem>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

class FileManager{
public:
  FileManager() = default;

  auto does_directory_exist() -> bool;
  void create_directory();  
 
  [[nodiscard]]
  auto get_user_path(std::string_view username) const -> fs::path;

  [[nodiscard]]
  auto does_user_exist(std::string_view username) const -> bool;

  void delete_user(std::string_view username) const;

  void write_user_data(std::string_view username, const std::vector<std::byte>& encrypted_data) const;
    

  [[nodiscard]]
  auto get_directory_path() -> fs::path{ return directory_path_; };  
  
private:  
  static auto find_directory_path() -> fs::path;

  fs::path directory_path_{find_directory_path()};

    
};



