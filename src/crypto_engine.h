#pragma once

#include "secure_buffer.h"
#include "constants.h"

#include <array>
#include <cstdint>
#include <sodium.h>
#include <string_view>
#include <span>

// class CryptoEngine{
// public:
//   CryptoEngine(std::string_view file_path) : file_path_{file_path} {}

//   void decrypt_to_buffer(SecureBuffer& secure_buffer, SecureBuffer& key);  
  
// private:


//   std::string file_path_;
// };


namespace CryptoEngine
{

  struct EncryptionDataRefView
  {
    // S;
  };
  
  auto generate_salt() -> std::array<uint8_t, constants::NUM_SALT_BYTES>;
  
  auto get_hashed_key(SecureBuffer& key, std::array<uint8_t, constants::NUM_SALT_BYTES>& salt) -> SecureBuffer;

  auto decrypt_file(SecureBuffer& key, std::string_view file_pat) -> SecureBuffer;

  void encrypt_file();
}

// decypt to secure buffer
// hash key
// encrypt file
