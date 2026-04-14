#pragma once

#include <memory>
#include <sodium/utils.h>
#include <string>
#include <string_view>
#include <vector>
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
    Search,
    Delete,
    Entry,
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


  // main vault helpers
  struct SearchState
  {
    std::string search_field; 
  }; 

  struct DeleteState
  {
    std::string delete_field; 
  }; 
 
  // main vault

  struct MainVaultState
  {
    std::shared_ptr<Vault> vault;

    int entry_selected{};
    std::vector<std::string_view> sites;
    const PasswordEntry* current_entry;
 
    bool is_revealed{false};
    
    void populate(Vault* vault);
  };

  struct EntryState
  {
    // fields
    std::string site;
    std::string username;
    std::string password;
    std::string notes;  

    // password generator
    std::string password_length { "16" }; // default password length is 16
    bool include_uppercase{true};
    bool include_lowercase{true};
    bool include_numbers  {true};
    bool include_symbols  {true};

    // same screen for both editing and new entries
    bool is_new{false};
  };

  
  struct AppState
  {
    SelectedScreen selected_screen{SelectedScreen::Start};
    
    MainVaultState main_vault{};
    EntryState entry{};
    LoginState login{};
    SetupState setup{};
    MessageState message{};
    SearchState search{};
    DeleteState deleter{};

    std::string password;
    
    void initalize();
    void destroy();
  };
}
