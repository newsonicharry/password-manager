#pragma once

#include <ftxui/component/app.hpp>

namespace ui::screens
{
  auto render_setup_screen(std::string* username, std::string* password, std::string* json_path, std::string* confirmed_password) -> ftxui::Component;

}
