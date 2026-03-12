#include "crypto_engine.h"
#include "constants.h"
#include "secure_buffer.h"
#include "utils.h"
#include "exception.h"
#include "vault_serializer.h"
#include <array>
#include <bit>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sodium/crypto_aead_aegis256.h>
#include <sodium.h>
#include <sodium/crypto_aead_aes256gcm.h>
#include <sodium/crypto_pwhash.h>
#include <iostream>
#include <span>
#include <vector>

namespace
{

// general helpers


auto get_additional_data(const FileHeaders& file_headers) -> std::vector<std::byte>
{ 
  std::vector<std::byte> additional_data;

  // loads the additional data
  back_insert_vec(additional_data, file_headers.nonce);
  back_insert_vec(additional_data, file_headers.salt);

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

auto decrypt_from_file_or_throw(const fs::path& file_path, const DecryptionDataRefView& decryption_data) -> SecureBuffer
{
  std::ifstream file{file_path, std::ios::binary};
  
  // start at the encrypted data
  file.seekg(protocol::TOTAL_HEADER_BYTES, std::ios::beg);

  const std::size_t total_size{ fs::file_size(decryption_data.file_path)};
  const std::size_t remaining_bytes{ total_size - protocol::TOTAL_HEADER_BYTES };
    
  if (remaining_bytes == 0 || decryption_data.file_headers.message_size == 0)
  {
    throw Exception("Encrypted data within file is empty.\n", Exception::ExceptionType::DecryptionError);
  }

  if (remaining_bytes != decryption_data.file_headers.message_size + crypto_aead_aegis256_ABYTES)
  {
    throw Exception("Incorrect number of bytes stored in file.\n", Exception::ExceptionType::DecryptionError);
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

// encryption helpers

template<typename T>
void write_binary(std::ofstream& file, const T& value)
{
  file.write(std::bit_cast<const char*>(&value), sizeof(value)); 
}


} // unnamed namespace

auto crypto_engine::hash_key(const SecureBuffer& password, std::span<const std::byte> salt) -> SecureBuffer
{
  assert(salt.size() == protocol::NUM_SALT_BYTES && "Salt must be exactly the size of the number of salt byte.");
  
  SecureBuffer hashed_key{protocol::NUM_KEY_HASH_BYTES};

  if (0 != crypto_pwhash(std::bit_cast<unsigned char*>(hashed_key.get_write_ptr()), hashed_key.size(),
                std::bit_cast<char*>(password.get_read_ptr()), password.size(),
                std::bit_cast<unsigned char*>(salt.data()),
                crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_DEFAULT))
  {
    throw Exception("Failed to hash password. Do you have enough memory?\n", Exception::ExceptionType::HashingError);
  }


  return hashed_key;
}



auto crypto_engine::decrypt_file(const fs::path& file_path, const FileHeaders& file_headers, const SecureBuffer& password) -> SecureBuffer
{

  SecureBuffer key{hash_key(password, file_headers.salt)};
  std::span<const std::byte> key_span{key.get_read_ptr(), key.size()};

  const DecryptionDataRefView decryption_data{file_path, file_headers, key_span};

  SecureBuffer decrypted_file {decrypt_from_file_or_throw(file_path, decryption_data)};

  return decrypted_file;
}


auto crypto_engine::encrypt_file(const fs::path& file_path, const SecureBuffer& message, const FileHeaders& file_headers, const SecureBuffer& password) -> std::vector<std::byte>
{

  SecureBuffer key{hash_key(password, file_headers.salt)};
  
  const std::vector<std::byte> additional_data{get_additional_data(file_headers)};

  std::vector<unsigned char> ciphertext(crypto_aead_aegis256_ABYTES+message.size());
  
  unsigned long long cipher_text_len{ciphertext.size()};

  if(0 != crypto_aead_aegis256_encrypt(ciphertext.data(), &cipher_text_len,
                             std::bit_cast<unsigned char*>(message.get_read_ptr()), message.size(),
                             std::bit_cast<unsigned char*>(additional_data.data()), additional_data.size(),
                             nullptr, std::bit_cast<unsigned char*>(file_headers.nonce.data()),
                             std::bit_cast<unsigned char*>(key.get_read_ptr())))
  {
    throw Exception("Failed to encrypt file.\n", Exception::ExceptionType::EncryptionError);
  }


  // the final data we are writing to file
  // this includes all the headers
  std::vector<std::byte> data_on_file;

  back_insert_vec(data_on_file, protocol::MAGIC_HEADER_NAME_VALUE);

  // additional data is simply the combined header excluding the magic name at the beginning
  back_insert_vec(data_on_file, additional_data); 

  back_insert_vec(data_on_file, ciphertext);

  return data_on_file;
}
