#include "screens.h"
#include "../theme.h"
#include "../components/components.h"
#include "../components/container.h"
#include "../components/input_field_component.h"
#include "ui_constants.h"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <memory>
#include <string_view>



using namespace ftxui;

namespace theme = ui::theme;
namespace constants = ui::screens::constants;
namespace state = ui::state;

constexpr std::array<std::string_view, 3> TITLE_TEXT{
    "██  ██ ███  ██ ██     ▄████▄ ▄█████ ██ ▄█▀   ██  ██ ▄████▄ ██  ██ ██    ██████",
    "██  ██ ██ ▀▄██ ██     ██  ██ ██     ████     ██▄▄██ ██▄▄██ ██  ██ ██      ██  ",
    "▀████▀ ██   ██ ██████ ▀████▀ ▀█████ ██ ▀█▄    ▀██▀  ██  ██ ▀████▀ ██████  ██  ",
};


namespace {

auto try_login(state::AppState& app_state)
{

  SecureBuffer password_buffer{app_state.login.password};
  auto vault{ Vault::open_existing(app_state.login.username, password_buffer) };

  if (!vault)
  {
    app_state.message.message = vault.error().what();
    app_state.message.message_type = MessageType::Error; 
    app_state.message.title = Exception::TYPE_TO_STRING.at(static_cast<std::size_t>(vault.error().type()));
    app_state.message.next_screen = ui::state::SelectedScreen::Login;

    app_state.selected_screen = ui::state::SelectedScreen::Message;
    return;
  }


  app_state.main_vault.vault = std::make_shared<Vault>(std::move(vault.value()));
  app_state.main_vault.populate(app_state.main_vault.vault.get());

  app_state.selected_screen = ui::state::SelectedScreen::MainVault;
}




auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components; 

  Filter username_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.login.username, constants::MAX_INPUT_CHARACTERS))};
  Filter password_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.login.password, constants::MAX_INPUT_CHARACTERS))};

  Component input_username{create_input_field(&app_state.login.username, "Enter your username...", username_filter)};
  Component input_password{create_input_field(&app_state.login.password, "Enter your master password...", password_filter, IS_PASSWORD_INPUT)};

  Component login_button{create_button("LOGIN", [&]{ try_login(app_state);}, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  Component back_button{create_button("BACK TO START", [&]{app_state.selected_screen = state::SelectedScreen::Start;}, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};

  auto components = Container::Vertical({
    input_username,
    input_password,
    login_button,
    back_button                    
  });
  
  return Renderer(components, [=]{
    return vbox({

      vbox({       
        separatorEmpty() | size(ftxui::HEIGHT, ftxui::EQUAL, 2),
        create_title_text(TITLE_TEXT),
        separatorEmpty(),                                                              
        text("Enter master password to decrypt and open") | color(theme::FONT_COLOR) | center,
      }) | flex,

      vbox({
        text("USERNAME") | center | color(theme::FONT_COLOR) | bold,
        hbox({
          separatorEmpty() | xflex,
          input_username->Render() | size(ftxui::WIDTH, ftxui::EQUAL, constants::MAX_INPUT_WIDTH),
          separatorEmpty() | xflex,
        }) | xflex,

        separatorEmpty() | size(ftxui::HEIGHT, EQUAL, 3),

        text("MASTER PASSWORD") | center | color(theme::FONT_COLOR) | bold,
        hbox({
          separatorEmpty() | xflex,
          input_password->Render() | size(ftxui::WIDTH, ftxui::EQUAL, constants::MAX_INPUT_WIDTH),
          separatorEmpty() | xflex,
        }) | xflex,

        vbox({separatorEmpty()}) | flex,

        vbox({         
          login_button->Render(),
          separatorEmpty(),
          back_button->Render(),
        }) | center | flex,

        vbox({separatorEmpty()}) | flex,
        
      }) | flex,
       
    }) | flex | bgcolor(theme::BODY_BG_COLOR);
  });
}

} // unnammed namespace


auto ui::screens::render_login_screen(state::AppState& app_state) -> Component
{
  
  Component header = components::render_header("UNLOCK VAULT");
  Component footer = components::render_footer("LOCKED", theme::WARNING_FONT_COLOR);
  Component body = render_body(app_state);
  
  Component layout = Container::Vertical({
    header,
    body,
    footer
  });
  
  return Renderer(layout, [=]{
     return vbox({
       header->Render(),
       body->Render(),
       footer->Render()                  
     }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);
  });
}




