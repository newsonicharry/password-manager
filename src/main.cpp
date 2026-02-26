#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <sodium.h>
#include <sodium/crypto_pwhash_argon2id.h>
#include <sodium/crypto_aead_aegis256.h>
#include <array>
#include <string_view>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include "constants.h"
#include "crypto_engine.h"
#include "exception.h"
#include "secure_buffer.h"
#include "file_manager.h"
#include "converter.h"

constexpr std::string_view PASSWORD{"PigeonsAreReallyCool12345!"};


template<typename T>
void write_binary(std::ofstream& file, const T& value)
{
  file.write(std::bit_cast<const char*>(&value), sizeof(value)); 
}

template<std::size_t N>
void write_binary(std::ofstream& file, const char(&value)[N])
{
  // make sure we dont include the null terminator
  file.write(value, N-1); 
}


using Nonce = std::array<std::byte, protocol::NUM_NONCE_BYTES>;
using Salt = std::array<std::byte, protocol::NUM_SALT_BYTES>;


void create_test_file(const fs::path& path)
{

  SecureBuffer password_holder{PASSWORD.length()};
  std::copy(PASSWORD.begin(), PASSWORD.end(), std::bit_cast<char*>(password_holder.get_write_ptr()));

  
  Salt salt{ crypto_engine::generate_random_buffer<protocol::NUM_SALT_BYTES>() };
  Nonce nonce{ crypto_engine::generate_random_buffer<protocol::NUM_NONCE_BYTES>() };

  

  SecureBuffer key{};
  try{
    key = crypto_engine::hash_key(password_holder, salt);  
  }
  catch (const char* e)
  {
    std::cout << e;
    std::exit(0);
  }
  


  constexpr std::array<std::byte, 16> MESSAGE
  {
    std::byte{static_cast<uint8_t>(protocol::MagicIdentifer::Initial)}, // start identifier
    std::byte{static_cast<uint8_t>(protocol::MagicIdentifer::Site)}, // site identifier

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
    std::byte{static_cast<uint8_t>(protocol::MagicIdentifer::Username)},

    //length
    std::byte{0}, 
    std::byte{3},

    // dog 
    std::byte{0x64}, 
    std::byte{0x6F}, 
    std::byte{0x67},
  };

  
  std::vector<std::byte> additional_data; 
  const uint16_t entry_count{1};
  const uint64_t message_size{MESSAGE.size()};

  std::copy(nonce.begin(), nonce.end(), std::back_inserter(additional_data));
  std::copy(salt.begin(), salt.end(), std::back_inserter(additional_data));
  additional_data.push_back(static_cast<std::byte>(1));
  // sketchy code but works for the sake of testing
  std::copy(std::bit_cast<std::byte*>(&entry_count), std::bit_cast<std::byte*>(&entry_count)+2, std::back_inserter(additional_data));
  std::copy(std::bit_cast<std::byte*>(&message_size), std::bit_cast<std::byte*>(&message_size)+8, std::back_inserter(additional_data));

  std::array<unsigned char, crypto_aead_aegis256_ABYTES+MESSAGE.size()> ciphertext{};
  
  unsigned long long cipher_text_len{ciphertext.size()};

  crypto_aead_aegis256_encrypt(ciphertext.data(), &cipher_text_len,
                             std::bit_cast<unsigned char*>(MESSAGE.data()), MESSAGE.size(),
                             std::bit_cast<unsigned char*>(additional_data.data()), additional_data.size(),
                             nullptr, std::bit_cast<unsigned char*>(nonce.data()),
                             std::bit_cast<unsigned char*>(key.get_read_ptr()));


 // write to file

  std::ofstream file{path, std::ios::binary};

  file.put('\0'); // null nerminator for the magic header
  write_binary(file, "Encrypt"); // rest of magic header
  write_binary(file, nonce); // nonce
  write_binary(file, salt); // salt

  write_binary(file, static_cast<uint8_t>(1)); // iterations
  write_binary(file, static_cast<uint16_t>(1)); // entry count

  write_binary(file, static_cast<uint64_t>(MESSAGE.size())); // file size
  
  write_binary(file, ciphertext); // actual encrypted data


}

auto main() -> int
{


  FileManager file_manager{};

  fs::path user_path{file_manager.get_user_path("default")};

  if (!file_manager.does_directory_exist())
  {
    file_manager.create_directory();
  }
  file_manager.delete_user("default");
  if (!file_manager.does_user_exist("default"))
  {
    create_test_file(user_path);  
  }
  
  SecureBuffer password{PASSWORD.length()};
  std::copy(PASSWORD.begin(), PASSWORD.end(), std::bit_cast<char*>(password.get_write_ptr()));

  try{
    crypto_engine::decrypt_file(user_path, password);
  }
  catch(const Exception& exception)
  {
    std::cout << exception.what();
  }

  try
  {
    SecureBuffer converted_bitwarden{convert_from_bitwarden_json("/home/harry-phillips/Downloads/export.json")};
  }
  catch(const Exception& exception)
  {
    std::cout << exception.what();
  }  
  
  
  return 0;
}



