#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include "app.h"
#include "screens/start_screen.h"
#include "screens/login_screen.h"

using namespace ftxui;

void ui::vault_renderer()
{
  auto username = std::make_shared<std::string>();
  auto password = std::make_shared<std::string>();

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  screen.Loop(screens::render_login_screen(username.get(), password.get()));
  
}




