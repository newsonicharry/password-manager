#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component_base.hpp>

namespace ui::screens
{
  auto render_login_screen(std::string* username, std::string* password) -> ftxui::Component;
}
