#include "constants.h"
#include "exception.h"
#include "file_manager.h"
#include "secure_buffer.h"
#include "vault.h"
#include "ui/app.h"
#include <filesystem>
#include <iostream>
#include <sodium/core.h>
#include <string_view>

constexpr std::string_view PASSWORD_STRING{"pigeonsarecool123"};


void create_new_vault_from_bitwarden(std::string_view username, std::string_view password_string)
{
  // auto vault {Vault::convert_from_bitwarden(username, fs::path("../tests/data/bitwarden_exports/valid/simple.json"))};
  auto vault {Vault::convert_from_bitwarden(username, fs::path("../../../Downloads/export.json"))};

  if (!vault)
  {
    std::cout << vault.error().what();
    return;
  }

  SecureBuffer password_buffer{password_string.length()};
  std::copy(std::bit_cast<std::byte*>(password_string.begin()), std::bit_cast<std::byte*>(password_string.end()), password_buffer.get_write_ptr());

  // vault->modify_entry(0, protocol::MagicIdentifier::Password, password_buffer);  
  vault->encrypt_to_file(password_buffer);
}

void create_new_vault(std::string_view username)
{
}

auto main() -> int
{
  if (sodium_init() < 0)
  {
    std::cout << "Failed to initalize libsodium.\n";
    return 1;
  }
  
  FileManager file_manager;
  file_manager.delete_user("harry");
  create_new_vault_from_bitwarden("harry", PASSWORD_STRING);  
  // open_existing_vault("harry", PASSWORD_STRING);
    
  ui::vault_renderer();
   
  return 0;
}



