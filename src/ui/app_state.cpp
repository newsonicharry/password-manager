#include "app_state.h"

void ui::state::MainVaultState::populate(Vault* vault)
{
  for (const auto& entry : vault->list_entries())
  {
    sites.push_back(entry.get_site());
  }

  current_entry = vault->list_entries().data();
  entry_selected = 0;
}


void ui::state::AppState::initalize()
{
  auto setup_secure_string{[](std::string& string){
    string.reserve(screens::constants::MAX_INPUT_CHARACTERS);
    sodium_mlock(string.data(), screens::constants::MAX_INPUT_CHARACTERS);
  }};

  setup_secure_string(login.password);
  setup_secure_string(setup.password);
  setup_secure_string(setup.confirmed_password);
  
}

void ui::state::AppState::destroy()
{
  auto destroy_secure_string{[](std::string& string){
    sodium_munlock(string.data(), string.length());
    sodium_memzero(string.data(), string.length());
  }};

  destroy_secure_string(login.password);
  destroy_secure_string(setup.password);
  destroy_secure_string(setup.confirmed_password);
}
