#pragma once

#include <ftxui/component/app.hpp>
#include "../app_state.h"

namespace ui::screens
{
  auto render_login_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_setup_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_start_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_vault_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_entry_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_search_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_delete_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_message_screen(state::AppState& app_state) -> ftxui::Component;  
}
