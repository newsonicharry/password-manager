#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <iostream>
#include <string_view>
#include <utility>
#include "app.h"
#include "app_state.h"
#include "password_utils.h"
#include "screens/screens.h"

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
  state::AppState app_state{};
  app_state.initalize();

  // const std::string my_password = "harry is super cool";
  // password_utils::PasswordStrength strength = password_utils::classify_password_strength(my_password);
  // std::cout << static_cast<int>(strength) << std::endl;
  // while(true);

  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  // screen.Loop(screens::render_setup_screen(app_state));
  Component start_screen{ screens::render_start_screen(app_state) };
  Component login_screen{ screens::render_login_screen(app_state) };
  Component setup_screen{ screens::render_setup_screen(app_state) };
  Component message_screen{ screens::render_message_screen(app_state)};


  auto all_screens{ Container::Vertical({
    start_screen | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Start; }),
    login_screen | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Login; }),
    setup_screen | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Setup; }),
    message_screen | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Message; })

  })};

  
  auto root{ Renderer(all_screens, [&]{

      switch (app_state.selected_screen){

      case state::SelectedScreen::Quit:
        screen.ExitLoopClosure()();

      case state::SelectedScreen::Login:
        return login_screen -> Render();

      case state::SelectedScreen::Setup:
        return setup_screen -> Render();

      case state::SelectedScreen::Message:        
        return message_screen -> Render();

      default:
        return start_screen -> Render();
      }
    }
  )};

  screen.Loop(root);  
  app_state.destroy();
}



