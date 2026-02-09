#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstring>
#include <sodium/crypto_pwhash_argon2id.h>
#include <sodium/crypto_aead_aegis256.h>
#include <array>
#include <string_view>
#include <fstream>
#include <iostream>

template<typename T>
void write_binary(std::ofstream& file, const T& value)
{
  file.write(std::bit_cast<const char*>(&value), sizeof(value)); 
}

auto main() -> int{
  // crypto_pwhash();

  constexpr std::string_view MESSAGE{"this is a super cool message that will never be leaked for sure!!!"};
  constexpr std::string_view ADDITIONAL_DATA{"publicdatahere"};
  std::array<unsigned char, crypto_aead_aegis256_NPUBBYTES> nonce{};
  std::array<unsigned char, crypto_aead_aegis256_KEYBYTES> key{}; 
  std::array<unsigned char, crypto_aead_aegis256_ABYTES+MESSAGE.length()> ciphertext{};

  std::cout << crypto_aead_aegis256_KEYBYTES << '\n'; 
  // randombytes_buf(key.data(), key.size());
  // randombytes_buf(nonce.data(), nonce.size());
  
  // used for the sake of testing and consistency, very bad for security
  std::fill(nonce.begin(), nonce.end(), 0);
  std::fill(key.begin(), key.end(), 0);

  
  unsigned long long cipher_text_len{ciphertext.size()};
  crypto_aead_aegis256_encrypt(ciphertext.data(), &cipher_text_len,
                             std::bit_cast<unsigned char*>(MESSAGE.data()), MESSAGE.length(),
                             std::bit_cast<unsigned char*>(ADDITIONAL_DATA.data()), ADDITIONAL_DATA.length(),
                             nullptr, nonce.data(), key.data());


  // write to file

  std::ofstream file{"output.encrypted", std::ios::binary};

  file.put('\0'); // null nerminator for the magic header
  write_binary(file, "Encrypt"); // rest of magic header
  write_binary(file, nonce); // nonce
  write_binary(file, nonce); // salt

  write_binary(file, static_cast<uint8_t>(0)); // iterations
  write_binary(file, static_cast<uint16_t>(0)); // entry count
  
  // decrypt


  std::array<unsigned char, MESSAGE.length()+1> decrypted{};
  decrypted[MESSAGE.length()] = 0; // null terminator at the end, should only be used for strings that are printed
  unsigned long long decrypted_len = MESSAGE.length();
  
  if (0 == crypto_aead_aegis256_decrypt(decrypted.data(), &decrypted_len,
                                 nullptr,
                                 ciphertext.data(), ciphertext.size(),
                                 std::bit_cast<unsigned char*>(ADDITIONAL_DATA.data()), ADDITIONAL_DATA.length(),
                                 nonce.data(), key.data()))
  {
    std::cout << decrypted.data() << '\n';
  }
  else
  {
    std::cout << "failed to decrypt\n";
  }

  return 0;
}



