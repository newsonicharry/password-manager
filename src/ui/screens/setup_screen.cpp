#include "screens.h"
#include "../theme.h"
#include "../components/components.h"
#include "../components/container.h"
#include "ui_constants.h"
#include <cctype>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>



using namespace ftxui;

namespace theme = ui::theme;
namespace constants = ui::screens::constants;
namespace state = ui::state;

constexpr std::array<std::string_view, 3> TITLE_TEXT{                                                                    
"██  ██ ▄████▄ ██  ██ ██    ██████   ▄█████ ██████ ██████ ██  ██ █████▄", 
"██▄▄██ ██▄▄██ ██  ██ ██      ██     ▀▀▀▄▄▄ ██▄▄     ██   ██  ██ ██▄▄█▀", 
" ▀██▀  ██  ██ ▀████▀ ██████  ██     █████▀ ██▄▄▄▄   ██   ▀████▀ ██    ", 
                                                                       
};


namespace {


// TODO: replace this with a secure buffer
auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components;  

  
  std::string* password_ptr{&app_state.setup.password};
  std::string* confirmed_password_ptr{&app_state.setup.confirmed_password};

  Filter username_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.setup.username, constants::MAX_INPUT_CHARACTERS))};
  Filter json_path_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.setup.json_path, constants::MAX_INPUT_CHARACTERS))};
  Filter password_filter{filter_combiner(newline_input_filter, char_limit_input_filter(password_ptr, constants::MAX_INPUT_CHARACTERS))};
  Filter confirmed_password_filter{filter_combiner(newline_input_filter, char_limit_input_filter(confirmed_password_ptr, constants::MAX_INPUT_CHARACTERS))};

  Component input_username{create_input_field(&app_state.setup.username, "Enter your username...", username_filter)};
  Component input_password{create_input_field(password_ptr, "Enter your master password...", password_filter, IS_PASSWORD_INPUT)};
  Component input_confirm_password{create_input_field(confirmed_password_ptr, "Confirm your master password...", confirmed_password_filter, IS_PASSWORD_INPUT)};
  Component input_json{create_input_field(&app_state.setup.json_path, "Enter your bitwarden export json...", json_path_filter, IS_PASSWORD_INPUT)};

  Component setup_button{create_button("CREATE VAULT", []{ std::cout << "Pressed"; }, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  Component back_button{create_button("BACK TO START", [&]{ app_state.selected_screen = state::SelectedScreen::Start; }, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};

  auto components = Container::Vertical({
    input_username,
    input_password,
    input_confirm_password,
    input_json,
    setup_button,
    back_button
  });
  
  return Renderer(components, [=]{
    return vbox({
      vbox({
        separatorEmpty() | size(ftxui::HEIGHT, ftxui::EQUAL, 2),
        create_title_text(TITLE_TEXT),
        separatorEmpty(),
        text("Vault configuration") | center | color(theme::FONT_COLOR)
      }),
      separatorEmpty() | flex,
      vbox({
        text("USERNAME") | color(theme::FONT_COLOR) | center | bold,
        input_username->Render() | size(ftxui::WIDTH, EQUAL, constants::MAX_INPUT_WIDTH) | center,
      }) | flex,
      vbox({
        text("NEW PASSWORD") | color(theme::FONT_COLOR) | center | bold,
        input_password->Render() | size(ftxui::WIDTH, EQUAL, constants::MAX_INPUT_WIDTH) | center,
        input_confirm_password->Render() | size(ftxui::WIDTH, EQUAL, constants::MAX_INPUT_WIDTH) | center,

      }) | flex,
      vbox({
        text("PASSWORD STRENGTH") | color(theme::FONT_COLOR) | center | bold,
        create_strength_bar(3, {Color::Red, Color::LightCoral, Color::Orange1, Color::LightGreen, Color::Green}) | size(ftxui::WIDTH, EQUAL, constants::MAX_INPUT_WIDTH) | center,
      }),
      separatorEmpty() | flex,
      vbox({
        text("BITWARDEN EXPORT JSON (OPTIONAL)") | color(theme::FONT_COLOR) | center | bold,
        input_json->Render() | size(ftxui::WIDTH, EQUAL, constants::MAX_INPUT_WIDTH) | center,
      }),
      separatorEmpty() | flex,

      vbox({
         setup_button->Render(), 
         back_button->Render(), 
       }),

      separatorEmpty() | flex,
    }) | flex | bgcolor(theme::BODY_BG_COLOR);
  });
}

} // unnammed namespace


auto ui::screens::render_setup_screen(state::AppState& app_state) -> Component
{
  
  Component header{ components::render_header("VAULT SETUP") };
  Component footer{ components::render_footer("INITALIZE", theme::FONT_COLOR) };

  Component body{ render_body(app_state) };
  
  Component layout { Container::Vertical({
    header,
    body,
    footer
  })};
  
  return Renderer(layout, [=]{
     return vbox({
       header->Render(),
       body->Render(),
       footer->Render()                  
     }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);;
  });
}


