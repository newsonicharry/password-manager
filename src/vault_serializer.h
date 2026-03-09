#pragma once

#include "secure_buffer.h"
#include "password_entry.h"
#include "constants.h"
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;


struct FileHeaders
{
  std::array<std::byte, protocol::NUM_NONCE_BYTES> nonce; 
  std::array<std::byte, protocol::NUM_SALT_BYTES> salt;
  uint8_t iterations;
  uint16_t entry_count;
  uint64_t message_size;
};


namespace vault_serializer
{


  auto parse_user_vault(const SecureBuffer& vault_data) -> std::vector<PasswordEntry>;

  auto generate_new_headers(const std::vector<PasswordEntry>& entries) -> FileHeaders; 

  auto get_headers_from_path(const fs::path& file_path) -> FileHeaders;    

  auto convert_entries_to_buffer(const std::vector<PasswordEntry>& entries) -> SecureBuffer;

  // general file_header struct

  // file parser
  // header creater
  // 

}
