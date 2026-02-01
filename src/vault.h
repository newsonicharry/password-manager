#ifndef VAULT_H
#define VAULT_H

#include "crypto_engine.h"

class Vault{
public:
  // Vault
  Vault(std::string_view username);
private:
  static void generate_directory();
  static auto does_directory_exist() -> bool;

  CryptoEngine vault_data_;
};


#endif
