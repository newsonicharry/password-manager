#pragma once

#include "exception.h"
#include "file_manager.h"
#include "password_entry.h"
#include "secure_buffer.h"
#include "vault_serializer.h"
#include <string_view>
#include <vector>
#include <expected>

//factory design
class Vault
{
public:  
  static auto create_new(std::string_view username) -> std::expected<Vault, Exception>;
  static auto open_existing(std::string_view username, const SecureBuffer& password) -> std::expected<Vault, Exception>;
  static auto convert_from_bitwarden(std::string_view username, const fs::path& json_path) -> std::expected<Vault, Exception>;

  [[nodiscard]]
  auto list_entries() const ->  const std::vector<PasswordEntry>& { return entries_; }
  void modify_entry(std::size_t index, protocol::MagicIdentifer identifier, const SecureBuffer& new_data);
  void add_entry(const PasswordEntry&);

  void encrypt_to_file(const SecureBuffer& password);

private:
  Vault(std::string_view username) : username_{username} { generate_dir_if_not_exists(); }

  std::vector<PasswordEntry> entries_;
  FileHeaders file_headers_{};
  std::string username_;
  FileManager file_manager_;

  void generate_dir_if_not_exists();
};


