#include "password_entry.h"
#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <string_view>


// helpers

auto PasswordEntry::get_string_from_entry(std::span<const std::byte> entry) -> std::string_view
{
  return std::string_view {std::bit_cast<char*>(entry.data()), entry.size()};
}

auto PasswordEntry::get_date_from_entry(std::span<const std::byte> entry) -> std::chrono::sys_seconds
{
  
  std::time_t created_date{};
  std::memcpy(&created_date, entry.data(), entry.size()); 

  std::chrono::sys_seconds time{std::chrono::seconds(created_date)};

  return time;
}

// member functions

auto PasswordEntry::get_site() const -> std::string_view
{
  return get_string_from_entry(entry_data_.site);
}

auto PasswordEntry::get_username() const -> std::string_view
{
  return get_string_from_entry(entry_data_.username);
}

auto PasswordEntry::get_password() const -> std::string_view
{
  return get_string_from_entry(entry_data_.password);
}

auto PasswordEntry::get_note() const -> std::string_view
{
  return get_string_from_entry(entry_data_.note);
}


auto PasswordEntry::get_date_created() const -> std::chrono::sys_seconds
{
  return get_date_from_entry(entry_data_.date_created);
}

auto PasswordEntry::get_date_modified() const -> std::chrono::sys_seconds
{
  return get_date_from_entry(entry_data_.date_modified);
}
