#pragma once

#include "secure_buffer.h"
#include "constants.h"
#include <array>
#include <cstdint>
#include <sodium.h>
#include <sodium/crypto_aead_aegis256.h>
#include <sodium/crypto_pwhash.h>
#include <stdexcept>
#include <string_view>
#include <span>
#include <filesystem>

namespace fs = std::filesystem;

namespace crypto_engine
{


class Error : public std::runtime_error
{
public:
  enum class ErrorType
  {
    HashingError,
    FileError,
  };

  Error(const std::string& error_msg, ErrorType error_type)
  : std::runtime_error(error_msg)
  , error_type_{error_type}
  {}

  [[nodiscard]]
  auto get_error_type() const -> ErrorType { return error_type_; }

private:
  ErrorType error_type_;
  
};


struct EncryptionDataRefView
{
  fs::path file_path;
  std::string_view message;
  std::string_view additional_data;
  std::span<std::array<unsigned char, crypto_aead_aegis256_NPUBBYTES>> nonce;
  std::span<SecureBuffer> key;
};

template <std::size_t N>
auto generate_random_buffer() -> std::array<uint8_t, N>
{  
  std::array<uint8_t, N> buffer{};

  randombytes_buf(buffer.data(), buffer.size());

  return buffer;
}


auto hash_key(SecureBuffer& password, std::array<uint8_t, protocol::NUM_SALT_BYTES>& salt) -> SecureBuffer;

auto decrypt_file(fs::path& file_path, const SecureBuffer& password) -> SecureBuffer;

void encrypt_file(const EncryptionDataRefView& encryption_data);


}

