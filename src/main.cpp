#include <cstring>
#include <fstream>
#include <sodium/crypto_pwhash_argon2id.h>

#define MESSAGE (const unsigned char *) "test\0"
#define MESSAGE_LEN 5
#define ADDITIONAL_DATA (const unsigned char *) "123456"
#define ADDITIONAL_DATA_LEN 6


#include <iostream>
#include <sodium/crypto_aead_aegis256.h>
#include "sodium.h"

auto main() -> int{
  // crypto_pwhash();

  unsigned char nonce[crypto_aead_aegis256_NPUBBYTES];
  unsigned char key[crypto_aead_aegis256_KEYBYTES];
  unsigned char ciphertext[MESSAGE_LEN + crypto_aead_aegis256_ABYTES];
  unsigned long long ciphertext_len;

  // crypto_aead_aegis256_keygen(key);
  // randombytes_buf(nonce, sizeof nonce);

  std::memset(key, 'A', crypto_aead_aegis256_KEYBYTES);
  std::memset(nonce, 'A', crypto_aead_aegis256_NPUBBYTES);
  crypto_aead_aegis256_encrypt(ciphertext, &ciphertext_len,
                             MESSAGE, MESSAGE_LEN,
                             ADDITIONAL_DATA, ADDITIONAL_DATA_LEN,
                             NULL, nonce, key);

  std::ofstream file{"output.txt"};
  file << ciphertext;
  
  // std::cout << ciphertext[0] << "it is end now";
  
  unsigned char decrypted[MESSAGE_LEN+1];
  decrypted[MESSAGE_LEN] = 0;
  std::memset(decrypted, 65, MESSAGE_LEN);
  std::cout << decrypted << '\n';
  unsigned long long decrypted_len;
  crypto_aead_aegis256_decrypt(decrypted, &decrypted_len,
                                 NULL,
                                 ciphertext, ciphertext_len,
                                 ADDITIONAL_DATA,
                                 ADDITIONAL_DATA_LEN,
                                 nonce, key);
  
  std::cout << decrypted;
 return 0;
}



