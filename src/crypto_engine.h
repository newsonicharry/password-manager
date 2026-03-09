#pragma once

#include "secure_buffer.h"
#include "constants.h"
#include "vault_serializer.h"
#include <array>
#include <cstddef>
#include <sodium.h>
#include <sodium/crypto_aead_aegis256.h>
#include <sodium/crypto_pwhash.h>
#include <span>
#include <filesystem>
#include <sys/types.h>

namespace fs = std::filesystem;

namespace crypto_engine
{


template <std::size_t N>
auto generate_random_buffer() -> std::array<std::byte, N>
{  
  std::array<std::byte, N> buffer{};

  randombytes_buf(buffer.data(), buffer.size());

  return buffer;
}


auto hash_key(const SecureBuffer& password, std::span<const std::byte> salt) -> SecureBuffer;

auto decrypt_file(const fs::path& file_path, const FileHeaders& file_headers, const SecureBuffer& password) -> SecureBuffer;

auto encrypt_file(const fs::path& file_path, const SecureBuffer& message, const FileHeaders& file_headers, const SecureBuffer& password) -> std::vector<std::byte>;
}

