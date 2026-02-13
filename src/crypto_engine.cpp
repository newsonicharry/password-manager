#include "crypto_engine.h"
#include "constants.h"
#include "secure_buffer.h"
#include <cstddef>
#include <cstdint>
#include <sodium/crypto_pwhash.h>



// auto crypto_engine::generate_nonce() -> std::array<uint8_t, constants::NUM_NONCE_BYTES>
// {  
//   std::array<uint8_t, constants::NUM_NONCE_BYTES> buffer{};
//   randombytes_buf(buffer.data(), buffer.size());
//   return buffer;
// }



auto crypto_engine::hash_key(SecureBuffer& key, std::array<uint8_t, crypto_pwhash_SALTBYTES>& salt) -> SecureBuffer
{
  SecureBuffer hashed_key{constants::NUM_KEY_HASH_BYTES};
  
  if (0 != crypto_pwhash(std::bit_cast<unsigned char*>(hashed_key.get_write_ptr()), hashed_key.get_length(),
                std::bit_cast<const char*>(key.get_read_ptr()), key.get_length(),
                salt.data(),
                crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_ARGON2ID13))
  {
    // throw "Failed to hash password. Do you have enough memory?";
  }


  return hashed_key;
}



auto crypto_engine::decrypt_file(const EncryptionDataRefView& encryption_data) -> SecureBuffer
{
  // encryption_data.file_path

  SecureBuffer decrypted_file{};
  
  return decrypted_file;
}


