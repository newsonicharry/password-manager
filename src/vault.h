#pragma once

#include "constants.h"
#include "crypto_engine.h"
#include "file_manager.h"
#include "password_entry.h"
#include "secure_buffer.h"
#include <cstddef>
#include <string_view>
#include <vector>

class Vault{
public:
  // Vault
  Vault();

  void generate_dir_if_not_exists();

  void create_new_user(std::string_view username);

  void try_decrypt_user_data(std::string_view username, const SecureBuffer& password);
  void try_encrypt_user_data() const;

  [[nodiscard]]
  auto list_entries() const ->  const std::vector<PasswordEntry>&;

  void modify_entry(std::size_t index, protocol::MagicIdentifer identifier, const SecureBuffer& new_data);
  
private:

  FileManager file_manager_{};
  std::vector<PasswordEntry> entries_{};

};




// things to do
// generate the highlevel directory if it doesnt exit
// try and open a file, then decrypt
// try and convert a file
// list entries
// have a high level modifier to said entires
// try and encrypt back a file


// get headers from path
