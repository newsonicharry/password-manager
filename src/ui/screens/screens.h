#pragma once

#include <ftxui/component/app.hpp>
#include "../../vault.h"
#include "../app_state.h"


namespace ui::screens
{
  auto render_login_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_setup_screen(std::string* username, std::string* password, std::string* json_path, std::string* confirmed_password) -> ftxui::Component;
  auto render_start_screen(state::AppState& app_state) -> ftxui::Component;
  auto render_vault_screen(const Vault& vault, ui::state::AppState& app_state) -> ftxui::Component;
  auto render_entry_screen(state::AppState& app_state) -> ftxui::Component;

  enum class MessageType{
    Info = 0,
    Success,
    Warning,
    Error,
    Size,
  };

  auto render_message_screen(std::string_view message, MessageType message_type, std::string_view title) -> ftxui::Component;  
}
