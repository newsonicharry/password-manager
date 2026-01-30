#ifndef CRYPTO_ENGINE_H
#define CRYPTO_ENGINE_H

#include "secure_string.h"
#include <sodium.h>

class CryptoEngine{
public:
  CryptoEngine(std::string_view path);

private:
  const std::string path_;
};

#endif 
