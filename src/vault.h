#ifndef VAULT_H
#define VAULT_H

#include "crypto_engine.h"

class Vault{
public:
  // Vault
  Vault(std::string_view username);
private:
  void generateDirectory();
  
  
  CryptoEngine vault_data;
};


#endif
