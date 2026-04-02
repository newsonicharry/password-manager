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
  };
  
}
