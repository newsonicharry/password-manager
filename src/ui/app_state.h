#pragma once

#include <sodium/utils.h>
#include <string>
#include <string_view>
#include <vector>
#include "screens/ui_constants.h"
#include "../vault.h"
#include "message_type.h"


namespace ui::state
{
  enum class SelectedScreen
  {
    Quit,
    Start,
    Login,
    Setup,
    Message,
    MainVault
  };



  struct LoginState
  {
    std::string username;
    std::string password;
  };

  struct SetupState
  {
    std::string username;
    std::string json_path;

    std::string password;
    std::string confirmed_password;

    int strength_bar_index{0};
  };

  struct MessageState
  {
    std::string message;
    MessageType message_type;
    std::string title;
    SelectedScreen next_screen;    
  };
  

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

  
  struct AppState
  {
    SelectedScreen selected_screen{SelectedScreen::Start};

    
    MainVaultState main_vault{};
    EditState editor{};
    LoginState login{};
    SetupState setup{};
    MessageState message{};

    void initalize();
    void destroy();
  };
}
