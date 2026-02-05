#pragma once

#include <ctime>
#include <span>
#include <string_view>
#include "secure_buffer.h"

class PasswordEntry{

public:
  PasswordEntry(const SecureBuffer& secure_buffer, std::size_t begin);

  auto get_site() -> std::string_view;
  auto get_username() -> std::string_view;
  auto get_password() -> std::string_view;
  auto get_notes() -> std::string_view;

  auto get_date_created() -> std::time_t; 
  auto get_date_modified() -> std::time_t;

 private:
  std::span<const std::byte> entry_data_;
};
