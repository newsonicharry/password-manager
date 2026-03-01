#pragma once

#include "secure_buffer.h"
#include "password_entry.h"
#include "constants.h"
#include <cstdint>

namespace fs = std::filesystem;


struct FileHeaders
{
  std::span<const std::byte, protocol::NUM_NONCE_BYTES> nonce; 
  std::span<const std::byte, protocol::NUM_SALT_BYTES> salt;
  uint8_t iterations;
  uint16_t entry_count;
  uint64_t message_size;
};


namespace vault_serializer
{


  auto parse_user_vault(const SecureBuffer& vault_data) -> std::vector<PasswordEntry>;

  auto generate_new_headers(const std::vector<PasswordEntry>& entries) -> FileHeaders; 
  
  // general file_header struct

  // file parser
  // header creater
  // 

}
