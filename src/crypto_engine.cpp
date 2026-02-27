#include "crypto_engine.h"
#include "constants.h"
#include "secure_buffer.h"
#include "utils.h"
#include "exception.h"
#include "vault_serializer.h"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sodium/crypto_pwhash.h>
#include <iostream>
#include <span>
#include <string_view>
#include <vector>

namespace
{

// general helpers


auto get_additional_data(const FileHeaders& file_headers) -> std::vector<std::byte>
{ 
  std::vector<std::byte> additional_data;

  // loads the additional data
  back_insert_vec(additional_data, file_headers.nonce.begin());
  back_insert_vec(additional_data, file_headers.salt.begin());

  back_insert_vec(additional_data, file_headers.iterations);
  back_insert_vec(additional_data, file_headers.entry_count);
  back_insert_vec(additional_data, file_headers.message_size);

  return additional_data;
}


// decryption helpers

struct DecryptionDataRefView
{
  fs::path file_path;
  FileHeaders file_headers;
  std::span<const std::byte> key;
};

auto open_password_file_or_throw(fs::path& file_path) -> std::ifstream
{

  if (!fs::exists(file_path))
  {
    throw Exception("File path for file to decypt does not exist.\n", Exception::ExceptionType::FileError);
  }
  
  std::ifstream file{file_path, std::ios::in | std::ios::binary};

  if (!file)
  {
    throw Exception("Failed to open and decrypt file.\n", Exception::ExceptionType::FileError);
  }

  const std::uintmax_t  size {fs::file_size(file_path)};

  if (size < protocol::TOTAL_HEADER_BYTES)
  {
    throw Exception("Header information from file does not exist.\n", Exception::ExceptionType::FileError);
  }


  std::string header_name(protocol::NUM_MAGIC_NAME_BYTES, '1');
  file.seekg(0, std::ios::beg);
  file.read(header_name.data(), protocol::NUM_MAGIC_NAME_BYTES);

  if (header_name != protocol::MAGIC_HEADER_NAME_VALUE)
  {
    throw Exception("Magic header does not match expected header.\n", Exception::ExceptionType::FileError);
  }

  return file;
}

auto decrypt_from_file_or_throw(std::ifstream& file, const DecryptionDataRefView& decryption_data) -> SecureBuffer
{
  // start at the encrypted data
  file.seekg(protocol::TOTAL_HEADER_BYTES, std::ios::beg);

  const std::size_t total_size{ fs::file_size(decryption_data.file_path)};
  const std::size_t remaining_bytes{ total_size - protocol::TOTAL_HEADER_BYTES };
    
  if (remaining_bytes == 0 || decryption_data.file_headers.message_size == 0)
  {
    throw Exception("Encrypted data within file is empty.\n", Exception::ExceptionType::DecryptionError);
  }
  
  std::vector<std::byte> ciphertext(remaining_bytes);  
  file.read(std::bit_cast<char*>(ciphertext.data()), static_cast<std::streamsize>(remaining_bytes));

  std::vector<std::byte> additional_data{get_additional_data(decryption_data.file_headers)};

  SecureBuffer decrypted_data{decryption_data.file_headers.message_size};
  unsigned long long num_decrypted_bytes{decryption_data.file_headers.message_size};
  
  if (0 != crypto_aead_aegis256_decrypt(std::bit_cast<unsigned char*>(decrypted_data.get_write_ptr()), &num_decrypted_bytes,
                                 nullptr,
                                 std::bit_cast<unsigned char*>(ciphertext.data()), ciphertext.size(),
                                 std::bit_cast<unsigned char*>(additional_data.data()), additional_data.size(),
                                 std::bit_cast<unsigned char*>(decryption_data.file_headers.nonce.data()),
                                 std::bit_cast<unsigned char*>(decryption_data.key.data())))
  {
    throw Exception("Failed to decrypt, likely due to an incorrect password.\n", Exception::ExceptionType::DecryptionError);
  }

  

  return decrypted_data;
}


} // unnamed namespace

auto crypto_engine::hash_key(const SecureBuffer& password, const std::array<std::byte, protocol::NUM_SALT_BYTES>& salt) -> SecureBuffer
{
  SecureBuffer hashed_key{protocol::NUM_KEY_HASH_BYTES};

  if (0 != crypto_pwhash(std::bit_cast<unsigned char*>(hashed_key.get_write_ptr()), hashed_key.size(),
                std::bit_cast<char*>(password.get_read_ptr()), password.size(),
                std::bit_cast<unsigned char*>(salt.data()),
                crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT))
  {
    throw Exception("Failed to hash password. Do you have enough memory?\n", Exception::ExceptionType::HashingError);
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
  uint64_t message_size{};

  file.read(std::bit_cast<char*>(nonce.data()), protocol::NUM_NONCE_BYTES);
  file.read(std::bit_cast<char*>(salt.data()), protocol::NUM_SALT_BYTES);

  file.read(std::bit_cast<char*>(&iterations), protocol::NUM_ITERATIONS_BYTES);
  file.read(std::bit_cast<char*>(&entry_count), protocol::NUM_ENTRY_COUNT_BYTES);
  file.read(std::bit_cast<char*>(&message_size), protocol::NUM_MESSAGE_SIZE_BYTES);

  SecureBuffer key{hash_key(password, salt)};
  std::span<const std::byte> key_span{key.get_read_ptr(), key.size()};

  const FileHeaders file_headers{nonce, salt, iterations, entry_count, message_size};
  const DecryptionDataRefView decryption_data{file_path, file_headers, key_span};

  SecureBuffer decrypted_file {decrypt_from_file_or_throw(file, decryption_data)};

  return decrypted_file;
}


