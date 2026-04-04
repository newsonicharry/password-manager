#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "../vault.h"

namespace ui::state
{
  struct MainVaultState
  {
    int entry_selected{};
    std::vector<std::string_view> sites;
    const PasswordEntry* current_entry;
    
    void populate(Vault& vault);
  };

  struct EditState
  {
    std::string site;
    std::string username;
    std::string email;
    std::string password;
    std::string notes;  

    // password generator
    std::string password_length { "16" }; // default password length is 16
    bool include_uppercase{true};
    bool include_lowercase{true};
    bool include_numbers  {true};
    bool include_symbols  {true};
  };

  enum class SelectedScreen
  {
    Start,
    Login,
    Setup,
    Message,
    MainVault
  };


  struct AppState
  {
    SelectedScreen selected_screen{SelectedScreen::Start};

    MainVaultState main_vault{};
    EditState editor{};
  };
  
}
