#pragma once

#include <expected>
#include <ftxui/component/app.hpp>
#include "../../vault.h"
#include "../app_state.h"

namespace ui::screens
{
  auto render_vault_screen(const Vault& vault, ui::state::AppState& app_state) -> ftxui::Component;
}
