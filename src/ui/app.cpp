#include <bitset>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include "app.h"
#include "app_state.h"
#include "screens/screens.h"
#include "password_utils.h"

using namespace ftxui;


constexpr std::string_view USERNAME{"harry"};
constexpr std::string_view PASSWORD_STRING{"pigeonsarecool123"};

auto open_existing_vault(std::string_view username, std::string_view password_string) -> Vault
{

  SecureBuffer password_buffer{password_string.length()};
  std::copy(std::bit_cast<std::byte*>(password_string.begin()), std::bit_cast<std::byte*>(password_string.end()), password_buffer.get_write_ptr());

  auto vault {Vault::open_existing(username, password_buffer)};

  if (!vault)
  {
    std::cout << vault.error().what();
  }

  return std::move(*vault);
}




void ui::vault_renderer()
{
  password_utils::generate_random_password(12, std::bitset<4>{"1111"});

  
  // while (true);

  state::AppState app_state{};
  Vault my_vault{open_existing_vault(USERNAME, PASSWORD_STRING)}; 

  app_state.main_vault.populate(my_vault);

  auto username = std::make_shared<std::string>();
  auto password = std::make_shared<std::string>();
  auto json_path = std::make_shared<std::string>();
  auto confirmed_password = std::make_shared<std::string>();

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  // screen.Loop(screens::render_setup_screen(username.get(), password.get(), json_path.get(), confirmed_password.get()));
  // screen.Loop(screens::render_message_screen("Hello, i am a very important message.", screens::MessageType::Warning, "Important Message Name"));
  // screen.Loop(screens::render_login_screen(username.get(), password.get()));

  // screen.Loop(screens::render_vault_screen(my_vault, app_state));
  screen.Loop(screens::render_entry_screen(app_state));  

}



