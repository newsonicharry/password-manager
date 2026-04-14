#include "screens.h"
#include "../theme.h"
#include "../components/components.h"
#include "../components/container.h"
#include "../components/input_field_component.h"
#include "../password_utils.h"
#include "ui_constants.h"
#include <cctype>
#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <functional>
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

// todo: combine this and the try_login into a central file
auto try_setup(state::AppState& app_state)
{
  
  if (app_state.setup.password != app_state.setup.confirmed_password)
  {
    app_state.message.message = "Password and confirmed password are not equal.\n";
    app_state.message.message_type = MessageType::Warning; 
    app_state.message.title = Exception::TYPE_TO_STRING.at(static_cast<std::size_t>(Exception::ExceptionType::UiError));
    app_state.message.next_screen = ui::state::SelectedScreen::Setup;

    app_state.selected_screen = ui::state::SelectedScreen::Message;
    return;
  }

  bool has_export{!app_state.setup.json_path.empty()};

  auto vault{ has_export ? Vault::convert_from_bitwarden(app_state.setup.username, fs::path{app_state.setup.json_path})
                         : Vault::create_new(app_state.setup.username)};

  if (!vault)
  {
    app_state.message.message = vault.error().what();
    app_state.message.message_type = MessageType::Error; 
    app_state.message.title = Exception::TYPE_TO_STRING.at(static_cast<std::size_t>(vault.error().type()));
    app_state.message.next_screen = ui::state::SelectedScreen::Setup;

    app_state.selected_screen = ui::state::SelectedScreen::Message;
    return;
  }
  
  app_state.password = app_state.setup.password;
  app_state.main_vault.vault = std::make_shared<Vault>(std::move(vault.value()));
  app_state.main_vault.populate(app_state.main_vault.vault.get());

  app_state.selected_screen = ui::state::SelectedScreen::MainVault;
}


auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components;  

  std::function<void()> on_password_update{
    [&](){
      ui::password_utils::PasswordStrength strength{ ui::password_utils::classify_password_strength(app_state.setup.password) };
      app_state.setup.strength_bar_index = static_cast<int>(strength);
  }};
  
  Filter username_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.setup.username, constants::MAX_INPUT_CHARACTERS))};
  Filter json_path_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.setup.json_path, constants::MAX_INPUT_CHARACTERS))};
  Filter password_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.setup.password, constants::MAX_INPUT_CHARACTERS))};
  Filter confirmed_password_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.setup.confirmed_password, constants::MAX_INPUT_CHARACTERS))};

  Component input_username{create_input_field(&app_state.setup.username, "Enter your username...", username_filter)};
  Component input_password{create_input_field(&app_state.setup.password, "Enter your master password...", password_filter, IS_PASSWORD_INPUT, on_password_update)};
  Component input_confirm_password{create_input_field(&app_state.setup.confirmed_password, "Confirm your master password...", confirmed_password_filter, IS_PASSWORD_INPUT)};
  Component input_json{create_input_field(&app_state.setup.json_path, "Enter your bitwarden export json...", json_path_filter)};

  Component setup_button{create_button("CREATE VAULT", [&]{ try_setup(app_state); }, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  Component back_button{create_button("BACK TO START", [&]{ app_state.selected_screen = state::SelectedScreen::Start; }, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};

  auto layout = Container::Vertical({
    input_username,
    input_password,
    input_confirm_password,
    input_json,
    setup_button,
    back_button
  });
  
  return Renderer(layout, [=, &layout, &app_state]{
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
        create_strength_bar(app_state.setup.strength_bar_index, {Color::Red, Color::LightCoral, Color::Orange1, Color::LightGreen, Color::Green}) | size(ftxui::WIDTH, EQUAL, constants::MAX_INPUT_WIDTH) | center,
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
  
  return Renderer(layout, [=, &app_state]{
     return vbox({
       header->Render(),
       body->Render(),
       footer->Render()                  
     }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);;
  });
}


