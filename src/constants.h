#pragma once


#include <cstddef>
#include <sodium/crypto_aead_aegis256.h>
#include <sodium/crypto_box.h>
#include <sodium/crypto_pwhash.h>
#include <string_view>
#include <sodium.h>

namespace constants
{
  inline constexpr std::string_view APP_NAME{ "password_manager" }; 
  inline constexpr std::size_t NUM_SALT_BYTES{ crypto_pwhash_SALTBYTES };  
  inline constexpr std::size_t NUM_KEY_HASH_BYTES{ crypto_box_SEEDBYTES };
  inline constexpr std::size_t NUM_NONCE_BYTES{ crypto_aead_aegis256_NPUBBYTES };
}


namespace magic_identifiers
{
  enum MAGIC_IDENTIFIERS
  {
    Initial = 0,

    Site = 1,
    Username = 2,
    Password = 3,
    Note = 4,
    DateCreated = 5,
    DatemModified = 6
  };
}
