#pragma once

#include "secure_buffer.h"
#include "file_manager.h"
#include "password_entry.h" 
#include <string_view>
#include <ctime>
#include "vector"


class VaultService{
public:
  VaultService() = default;

  [[nodiscard]]
  void create_dir_if_not_exists();


  [[nodiscard]]
  auto try_get_user_data(std::string_view username) -> bool;
  
  
private:
  
  SecureBuffer secure_buffer_{};
  FileManager file_manager_{};
  std::vector<PasswordEntry> password_entries_{};
};

