#pragma once

#include <chrono>
#include <cstddef>
#include <ctime>
#include <span>
#include <string_view>
#include "secure_buffer.h"


struct Offsets
{
  std::span<std::byte> site;
  std::span<std::byte> username;
  std::span<std::byte> password;
  std::span<std::byte> note;
  std::span<std::byte> date_created;
  std::span<std::byte> date_modified;
};

class PasswordEntry{

public:
  
  PasswordEntry(const SecureBuffer& secure_buffer, Offsets entry_data)
  : entry_data_{entry_data}
  {}

  auto get_site() const -> std::string_view;
  auto get_username() const -> std::string_view;
  auto get_password() const -> std::string_view;
  auto get_note() const -> std::string_view;

  auto get_date_created() const -> std::chrono::sys_seconds; 
  auto get_date_modified() const -> std::chrono::sys_seconds;

 private:
  Offsets entry_data_;

  static auto get_string_from_entry(std::span<std::byte> entry) -> std::string_view;
  static auto get_date_from_entry(std::span<std::byte> entry) -> std::chrono::sys_seconds;
};
