#pragma once

#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <span>
#include <string_view>
#include "constants.h"
#include "exception.h"
#include "secure_buffer.h"

using MagicIdentifier = protocol::MagicIdentifer;

struct Offsets
{
  std::span<const std::byte> site;
  std::span<const std::byte> username;
  std::span<const std::byte> password;
  std::span<const std::byte> note;
  std::span<const std::byte> date_created;
  std::span<const std::byte> date_modified;

  void set(MagicIdentifier identifier, std::span<const std::byte> value)
  {
    switch (identifier) {
      case MagicIdentifier::Site:          site          = value; break;
      case MagicIdentifier::Username:      username      = value; break;
      case MagicIdentifier::Password:      password      = value; break;
      case MagicIdentifier::Note:          note          = value; break;
      case MagicIdentifier::DateCreated:   date_created  = value; break;
      case MagicIdentifier::DatemModified: date_modified = value; break;

      // its expected that the identifier has already been confirmed to be valid before hand
      default:
        throw Exception("Invalid identifier given.\n", Exception::ExceptionType::ParsingError);
    }   
  }

};

class PasswordEntry{

public:
  
  PasswordEntry(const SecureBuffer& secure_buffer, const Offsets& entry_data)
  : entry_data_{entry_data}
  {}

  [[nodiscard]] auto get_site() const -> std::string_view;
  [[nodiscard]] auto get_username() const -> std::string_view;
  [[nodiscard]] auto get_password() const -> std::string_view;
  [[nodiscard]] auto get_note() const -> std::string_view;

  [[nodiscard]] auto get_date_created() const -> std::chrono::sys_seconds; 
  [[nodiscard]] auto get_date_modified() const -> std::chrono::sys_seconds;

 private:
  Offsets entry_data_{};

  static auto get_string_from_entry(std::span<const std::byte> entry) -> std::string_view;
  static auto get_date_from_entry(std::span<const std::byte> entry) -> std::chrono::sys_seconds;
};
