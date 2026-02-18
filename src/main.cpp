#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sodium.h>
#include <sodium/crypto_pwhash_argon2id.h>
#include <sodium/crypto_aead_aegis256.h>
#include <array>
#include <span>
#include <string_view>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include "constants.h"
#include "crypto_engine.h"
#include "secure_buffer.h"

template<typename T>
void write_binary(std::ofstream& file, const T& value)
{
  file.write(std::bit_cast<const char*>(&value), sizeof(value)); 
}

using Nonce = std::array<uint8_t, constants::NUM_NONCE_BYTES>;
using Salt = std::array<uint8_t, constants::NUM_SALT_BYTES>;

auto main() -> int{
  // crypto_pwhash();

  // std::cout << 

  constexpr std::string_view PASSWORD{"PigeonsAreReallyCool12345!"};

  SecureBuffer password_holder{PASSWORD.length()};
  std::copy(PASSWORD.begin(), PASSWORD.end(), std::bit_cast<char*>(password_holder.get_write_ptr()));

  
  Salt salt{ crypto_engine::generate_random_buffer<constants::NUM_SALT_BYTES>() };
  Nonce nonce{ crypto_engine::generate_random_buffer<constants::NUM_NONCE_BYTES>() };

  
  std::cout << sizeof(salt) << '\n'; 
  std::cout << sizeof(nonce) << '\n';

  SecureBuffer key{};
  try{
    key = crypto_engine::hash_key(password_holder, salt);  
  }
  catch (const char* e)
  {
    std::cout << e;
    std::exit(0);
  }
  std::cout << "testing " << key.get_length() << '\n';
  
  std::cout << magic_identifiers::Site;

  std::cout << "finished 1\n";

  constexpr std::array<std::byte, 16> MESSAGE
  {
    std::byte{magic_identifiers::Initial}, // start identifier
    std::byte{magic_identifiers::Site}, // site identifier

    std::byte{0}, // length 1
    std::byte{6}, // length 2

    // github
    std::byte{0x67}, 
    std::byte{0x69}, 
    std::byte{0x74},
    std::byte{0x68},
    std::byte{0x75},
    std::byte{0x62},

    // username identifer
    std::byte{magic_identifiers::Username},

    //length
    std::byte{0}, 
    std::byte{3},

    // dog 
    std::byte{0x64}, 
    std::byte{0x6F}, 
    std::byte{0x67},
  };

  
  constexpr std::string_view ADDITIONAL_DATA{"publicdatahere"};
  std::array<unsigned char, crypto_aead_aegis256_ABYTES+MESSAGE.size()> ciphertext{};

  
  std::cout << "finished 2\n";
  unsigned long long cipher_text_len{ciphertext.size()};

  std::cout << key.get_length() << '\n';
  std::cout << crypto_aead_aegis256_KEYBYTES << '\n';
  crypto_aead_aegis256_encrypt(ciphertext.data(), &cipher_text_len,
                             std::bit_cast<unsigned char*>(MESSAGE.data()), MESSAGE.size(),
                             std::bit_cast<unsigned char*>(ADDITIONAL_DATA.data()), ADDITIONAL_DATA.length(),
                             nullptr, nonce.data(),
                             std::bit_cast<unsigned char*>(key.get_read_ptr()));

  
  std::cout << "finished 3\n";
 // write to file

  std::ofstream file{"output.encrypted", std::ios::binary};

  file.put('\0'); // null nerminator for the magic header
  write_binary(file, "Encrypt"); // rest of magic header
  write_binary(file, nonce); // nonce
  write_binary(file, salt); // salt

  write_binary(file, static_cast<uint8_t>(1)); // iterations
  write_binary(file, static_cast<uint16_t>(1)); // entry count

  write_binary(file, ciphertext); // actuall encrypted data


  // std::array<unsigned char, MESSAGE.size()+1> decrypted{};
  // decrypted[MESSAGE.length()] = 0; // null terminator at the end, should only be used for strings that are printed
  // unsigned long long decrypted_len = MESSAGE.length();
  
  // if (0 == crypto_aead_aegis256_decrypt(decrypted.data(), &decrypted_len,
  //                                nullptr,
  //                                ciphertext.data(), ciphertext.size(),
  //                                std::bit_cast<unsigned char*>(ADDITIONAL_DATA.data()), ADDITIONAL_DATA.length(),
  //                                nonce.data(),
  //                                std::bit_cast<unsigned char*>(key.get_read_ptr())))
  // {
  //   std::cout << decrypted.data() << '\n';
  // }
  // else
  // {
  //   std::cout << "failed to decrypt\n";
  // }

  return 0;
}



