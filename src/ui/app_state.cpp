#include "app_state.h"

void ui::state::MainVaultState::populate(Vault& vault)
{
  for (const auto& entry : vault.list_entries())
  {
    sites.push_back(entry.get_site());
  }

  current_entry = vault.list_entries().data();
  entry_selected = 0;
}
