#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include "app.h"
#include "screens/start_screen.h"
#include "screens/login_screen.h"
#include "screens/setup_screen.h"
#include "screens/message_screen.h"

using namespace ftxui;

void ui::vault_renderer()
{
  auto username = std::make_shared<std::string>();
  auto password = std::make_shared<std::string>();
  auto json_path = std::make_shared<std::string>();
  auto confirmed_password = std::make_shared<std::string>();

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  // screen.Loop(screens::render_setup_screen(username.get(), password.get(), json_path.get(), confirmed_password.get()));
  screen.Loop(screens::render_message_screen("Hello, i am a very important message.", screens::MessageType::Warning, "Important Message Name"));
  // screen.Loop(screens::render_login_screen(username.get(), password.get()));
  
}




