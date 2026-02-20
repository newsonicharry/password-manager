#pragma once


#include <cstddef>
#include <cstdint>
#include <sodium/crypto_aead_aegis256.h>
#include <sodium/crypto_box.h>
#include <sodium/crypto_pwhash.h>
#include <string_view>
#include <sodium.h>

namespace project
{
  inline constexpr std::string_view APP_NAME{ "password-manager" }; 

  inline constexpr std::string_view PASSWORD_LIST_DIR{ "passwords-list" };
  inline constexpr std::string_view FILE_EXTENSION{ ".encrypted" };
}

namespace protocol
{
  inline constexpr std::size_t NUM_MAGIC_NAME_BYTES{ 8 };
  inline constexpr std::size_t NUM_ITERATIONS_BYTES{ 1 };
  inline constexpr std::size_t NUM_ENTRY_COUNT_BYTES{ 2 };
  inline constexpr std::size_t NUM_SALT_BYTES{ crypto_pwhash_SALTBYTES };  
  inline constexpr std::size_t NUM_NONCE_BYTES{ crypto_aead_aegis256_NPUBBYTES };

  
  inline constexpr std::string_view MAGIC_HEADER_NAME_VALUE{"\0Encrypt", NUM_MAGIC_NAME_BYTES};
  inline constexpr std::size_t TOTAL_HEADER_BYTES{  NUM_MAGIC_NAME_BYTES
                                                  + NUM_ITERATIONS_BYTES
                                                  + NUM_ENTRY_COUNT_BYTES
                                                  + NUM_SALT_BYTES
                                                  + NUM_NONCE_BYTES};

  inline constexpr std::size_t NUM_KEY_HASH_BYTES{ crypto_aead_aegis256_KEYBYTES };

  

  enum class MagicIdentifer : std::uint8_t
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


