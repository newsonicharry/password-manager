#pragma once

#include "secure_buffer.h"
#include "constants.h"
#include <array>
#include <cstddef>
#include <sodium.h>
#include <sodium/crypto_aead_aegis256.h>
#include <sodium/crypto_pwhash.h>
#include <string_view>
#include <span>
#include <filesystem>

namespace fs = std::filesystem;

namespace crypto_engine
{



struct EncryptionDataRefView
{
  fs::path file_path;
  std::string_view message;
  std::string_view additional_data;
  std::span<const std::byte> nonce;
  std::span<const std::byte> key;
};

template <std::size_t N>
auto generate_random_buffer() -> std::array<std::byte, N>
{  
  std::array<std::byte, N> buffer{};

  randombytes_buf(buffer.data(), buffer.size());

  return buffer;
}


auto hash_key(const SecureBuffer& password, const std::array<std::byte, protocol::NUM_SALT_BYTES>& salt) -> SecureBuffer;

auto decrypt_file(fs::path& file_path, const SecureBuffer& password) -> SecureBuffer;

void encrypt_file(const EncryptionDataRefView& encryption_data);


}

