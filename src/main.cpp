#include "ui/app.h"
#include <iostream>
#include <sodium/core.h>

auto main() -> int
{
  if (sodium_init() < 0)
  {
    std::cout << "Failed to initalize libsodium.\n";
    return 1;
  }
     
  ui::vault_renderer();
   
  return 0;
}



