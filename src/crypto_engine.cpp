#include "crypto_engine.h"
#include "constants.h"
#include "secure_buffer.h"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sodium/crypto_pwhash.h>
#include <iostream>



auto crypto_engine::hash_key(SecureBuffer& password, std::array<uint8_t, protocol::NUM_SALT_BYTES>& salt) -> SecureBuffer
{
  SecureBuffer hashed_key{protocol::NUM_KEY_HASH_BYTES};

  std::cout << std::bit_cast<char*>(password.get_read_ptr());
  std::cout << password.get_length();
  if (0 != crypto_pwhash(std::bit_cast<unsigned char*>(hashed_key.get_write_ptr()), hashed_key.get_length(),
                std::bit_cast<const char*>(password.get_read_ptr()), password.get_length(),
                salt.data(),
                crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT))
  {
    throw "Failed to hash password. Do you have enough memory?\n";
  }


  return hashed_key;
}



auto crypto_engine::decrypt_file(const EncryptionDataRefView& encryption_data) -> SecureBuffer
{

  if (!fs::exists(encryption_data.file_path))
  {
    throw "File path for file to decypt does not exist.\n";
  }
  
  std::ifstream file{encryption_data.file_path, std::ios::in | std::ios::binary};

  if (!file)
  {
    throw "Failed to open and decrypt file.\n";
  }

  const std::size_t size {fs::file_size(encryption_data.file_path)};

  if (size < protocol::TOTAL_HEADER_BYTES)
  {
    throw "Header information from file does not exist.";
  }


  std::string header_name(protocol::NUM_HEADER_NAME_BYTES, '\0');
  file.seekg(0, std::ios::beg);
  file.read(header_name.data(), protocol::NUM_HEADER_NAME_BYTES);

  if (header_name != protocol::HEADER_NAME_VALUE)
  {
    throw "Magic header does not match expected header.\n";
  }

  std::array<std::byte, protocol::NUM_NONCE_BYTES> nonce{}; 
  std::array<std::byte, protocol::NUM_SALT_BYTES> salt{};

  uint8_t iterations{};
  uint16_t entry_count{};

  file.read(std::bit_cast<char*>(nonce.data()), protocol::NUM_NONCE_BYTES);
  file.read(std::bit_cast<char*>(salt.data()), protocol::NUM_SALT_BYTES);

  file >> iterations;
  file >> entry_count;  
  
  SecureBuffer decrypted_file{};


  file.close();  
  return decrypted_file;


}


