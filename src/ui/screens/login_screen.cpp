#include "login_screen.h"
#include "../theme.h"
#include "../components/components.h"
#include <cstddef>
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

constexpr std::array<std::string_view, 3> TITLE_TEXT{
    "██  ██ ███  ██ ██     ▄████▄ ▄█████ ██ ▄█▀   ██  ██ ▄████▄ ██  ██ ██    ██████",
    "██  ██ ██ ▀▄██ ██     ██  ██ ██     ████     ██▄▄██ ██▄▄██ ██  ██ ██      ██  ",
    "▀████▀ ██   ██ ██████ ▀████▀ ▀█████ ██ ▀█▄    ▀██▀  ██  ██ ▀████▀ ██████  ██  ",
};

constexpr std::size_t TITLE_TEXT_WIDTH{TITLE_TEXT[0].length()-100};
constexpr std::size_t STANDARD_WIDTH{static_cast<std::size_t>(TITLE_TEXT_WIDTH*0.8)};

constexpr std::size_t MAX_INPUT_CHARACTERS{50};
constexpr std::size_t MAX_INPUT_WIDTH{STANDARD_WIDTH};
constexpr std::size_t MAX_BUTTON_WIDTH{STANDARD_WIDTH};


namespace {


//helpers

auto create_title_text() -> Element
{
  return vbox({
    text(TITLE_TEXT[0]) | color(theme::FONT_COLOR) | center, 
    text(TITLE_TEXT[1]) | color(theme::FONT_COLOR) | center, 
    text(TITLE_TEXT[2]) | color(theme::FONT_COLOR) | center,                            
  });
}



// renderers
auto render_footer() -> Component
{
  return Renderer([]{
    return vbox({
      separator(),
      hbox({
        hbox({
          separatorEmpty(),
          text("STATUS ") | color(theme::FONT_COLOR),
          text("LOCKED") | color(theme::WARNING_FONT_COLOR) | bold,
          separatorEmpty(),
          separator(),           
        }),
        hbox({
          separator(),           
          separatorEmpty(),
          text("ENTER ") | color(theme::FONT_COLOR) | bold,
          text("SELECT") | color(theme::FONT_COLOR),
          separatorEmpty(),
        }) | align_right | flex,
        hbox({
          separator(),           
          separatorEmpty(),
          text("TAB ") | color(theme::FONT_COLOR) | bold,
          text("NEXT FIELD") | color(theme::FONT_COLOR),
          separatorEmpty(),
        }) | align_right,

      }) | xflex_grow,

    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

auto render_header() -> Component
{
  return Renderer([]{
    return vbox({
      hbox({
        text("VAULTTUI ") | color(theme::ALT_FONT_COLOR) | bold,       
        text("- UNLOCK VAULT") | color(theme::FONT_COLOR)       
      }),
      separator()
    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

// TODO: replace this with a secure buffer
auto render_body(std::string* username, std::string* password) -> Component
{
  using namespace ui::components;  

  Filter username_filter{filter_combiner(newline_input_filter, char_limit_input_filter(username, MAX_INPUT_CHARACTERS))};
  Filter password_filter{filter_combiner(newline_input_filter, char_limit_input_filter(password, MAX_INPUT_CHARACTERS))};

  Component input_username{create_input_field(username, "Enter your username...", username_filter)};
  Component input_password{create_input_field(password, "Enter your master password...", password_filter, IS_PASSWORD_INPUT)};

  Component login_button{create_button("LOGIN", []{ std::cout << "Pressed"; }, MAX_BUTTON_WIDTH)};
  Component setup_button{create_button("SETUP", []{ std::cout << "Pressed"; }, MAX_BUTTON_WIDTH)};

  auto components = Container::Vertical({
    input_username,
    input_password,
    login_button,
    setup_button                    
  });
  
  return Renderer(components, [=]{
    return vbox({

      vbox({       
        separatorEmpty(),
        separatorEmpty(),
        create_title_text(),
        separatorEmpty(),                                                              
        text("Enter master password to decrypt and open") | color(theme::FONT_COLOR) | center,
      }) | flex,

      vbox({
        text("USERNAME") | center | color(theme::FONT_COLOR) | bold,
        hbox({
          separatorEmpty() | xflex,
          input_username->Render() | size(ftxui::WIDTH, ftxui::EQUAL, MAX_INPUT_WIDTH),
          separatorEmpty() | xflex,
        }) | xflex,

        separatorEmpty() | size(ftxui::HEIGHT, EQUAL, 3),

        text("MASTER PASSWORD") | center | color(theme::FONT_COLOR) | bold,
        hbox({
          separatorEmpty() | xflex,
          input_password->Render() | size(ftxui::WIDTH, ftxui::EQUAL, MAX_INPUT_WIDTH),
          separatorEmpty() | xflex,
        }) | xflex,

        vbox({separatorEmpty()}) | flex,

        vbox({         
          login_button->Render(),
          separatorEmpty(),
          setup_button->Render(),
        }) | center | flex,

        vbox({separatorEmpty()}) | flex,
        
      }) | flex,
       
    }) | flex | bgcolor(theme::BODY_BG_COLOR);
  });
}

} // unnammed namespace


auto ui::screens::render_login_screen(std::string* username, std::string* password) -> Component
{
  
  Component header = render_header();
  Component body = render_body(username, password);
  Component footer = render_footer();
  
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
     }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);;
  });
}




