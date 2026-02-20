#include "crypto_engine.h"
#include "constants.h"
#include "secure_buffer.h"
#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sodium/crypto_pwhash.h>
#include <iostream>

namespace
{

// decryption helpers
auto open_password_file_or_throw(fs::path& file_path) -> std::ifstream
{
  using Error = crypto_engine::Error;

  if (!fs::exists(file_path))
  {
    throw Error("File path for file to decypt does not exist.\n", Error::ErrorType::FileError);
  }
  
  std::ifstream file{file_path, std::ios::in | std::ios::binary};

  if (!file)
  {
    throw Error("Failed to open and decrypt file.\n", Error::ErrorType::FileError);
  }

  const std::uintmax_t  size {fs::file_size(file_path)};

  if (size < protocol::TOTAL_HEADER_BYTES)
  {
    throw Error("Header information from file does not exist.\n", Error::ErrorType::FileError);
  }


  std::string header_name(protocol::NUM_MAGIC_NAME_BYTES, '1');
  file.seekg(0, std::ios::beg);
  file.read(header_name.data(), protocol::NUM_MAGIC_NAME_BYTES);

  if (header_name != protocol::MAGIC_HEADER_NAME_VALUE)
  {
    throw Error("Magic header does not match expected header.\n", Error::ErrorType::FileError);
  }

  return file;
}


void load_entry_from_file_or_throw(std::ifstream& file)
{
  file.seekg(protocol::TOTAL_HEADER_BYTES, std::ios::beg);

  

  
}


}

auto crypto_engine::hash_key(SecureBuffer& password, std::array<uint8_t, protocol::NUM_SALT_BYTES>& salt) -> SecureBuffer
{
  SecureBuffer hashed_key{protocol::NUM_KEY_HASH_BYTES};

  if (0 != crypto_pwhash(std::bit_cast<unsigned char*>(hashed_key.get_write_ptr()), hashed_key.get_length(),
                std::bit_cast<const char*>(password.get_read_ptr()), password.get_length(),
                salt.data(),
                crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT))
  {
    throw Error("Failed to hash password. Do you have enough memory?\n", Error::ErrorType::HashingError);
  }


  return hashed_key;
}



auto crypto_engine::decrypt_file(fs::path& file_path, const SecureBuffer& password) -> SecureBuffer
{
  std::ifstream file{open_password_file_or_throw(file_path)};
  
  std::array<std::byte, protocol::NUM_NONCE_BYTES> nonce{}; 
  std::array<std::byte, protocol::NUM_SALT_BYTES> salt{};
  
  uint8_t iterations{};
  uint16_t entry_count{};

  file.read(std::bit_cast<char*>(nonce.data()), protocol::NUM_NONCE_BYTES);
  file.read(std::bit_cast<char*>(salt.data()), protocol::NUM_SALT_BYTES);

  file.read(std::bit_cast<char*>(&iterations), protocol::NUM_ITERATIONS_BYTES);
  file.read(std::bit_cast<char*>(&entry_count), protocol::NUM_ENTRY_COUNT_BYTES);

  SecureBuffer decrypted_file{};

  file.close();  
  return decrypted_file;


}


