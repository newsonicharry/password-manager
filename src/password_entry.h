#pragma once

#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <span>
#include <string_view>
#include "constants.h"
#include "secure_buffer.h"

using MagicIdentifier = protocol::MagicIdentifer;

struct Slices
{
  std::span<const std::byte> site;
  std::span<const std::byte> username;
  std::span<const std::byte> password;
  std::span<const std::byte> note;
  std::span<const std::byte> date_created;
  std::span<const std::byte> date_modified;

  [[nodiscard]]
  auto get(MagicIdentifier identifier) const -> std::span<const std::byte>;
  void set(MagicIdentifier identifier, std::span<const std::byte> value);

  [[nodiscard]]
  auto get_num_bytes() const -> std::size_t;
};

class PasswordEntry{

public:
  
  PasswordEntry(const SecureBuffer& vault, const Slices& vault_offsets);

  [[nodiscard]] auto get_site() const -> std::string_view;
  [[nodiscard]] auto get_username() const -> std::string_view;
  [[nodiscard]] auto get_password() const -> std::string_view;
  [[nodiscard]] auto get_note() const -> std::string_view;

  [[nodiscard]] auto get_date_created() const -> std::chrono::sys_seconds; 
  [[nodiscard]] auto get_date_modified() const -> std::chrono::sys_seconds;

  [[nodiscard]] auto get_num_bytes_stored() const -> std::size_t{ return entry_.size(); }
 private:
  Slices entry_offset_{};
  SecureBuffer entry_{};
  
  static auto get_string_from_entry(std::span<const std::byte> entry) -> std::string_view;
  static auto get_date_from_entry(std::span<const std::byte> entry) -> std::chrono::sys_seconds;
};
