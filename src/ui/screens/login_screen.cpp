#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include "login_screen.h"
#include "../theme.h"
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace ftxui;
namespace theme = ui::theme;

namespace {
//helpers
auto render_footer() -> Component
{
  return Renderer([]{
    return vbox({
      separator(),
      hbox({
        separatorEmpty(),
        // text("STATUS ") | color(theme::FONT_COLOR),
        // text("LOCKED") | color(theme::WARNING_FONT_COLOR),
        // separatorEmpty(),
        // separator(),           
      }),
    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

auto render_header() -> Component
{
  return Renderer([]{
    return vbox({
      hbox({
        text("VAULTTUI ") | color(theme::ALT_FONT_COLOR),       
        text("- UNLOCK VAULT") | color(theme::FONT_COLOR)       
      }),
      separator()
    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

// TODO: replace this with a secure buffer
auto render_body(std::string* username, std::string* password) -> Component
{
  Component input_username = Input(username, "Enter your username...");
  input_username |= CatchEvent([=](const Event& event){
    return event.is_character() && static_cast<int>(username->length()) >= 50;                            
  });
  
  
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(password, "Enter your master password...", password_option);
  input_password |= CatchEvent([=](const Event& event){
    return event.is_character() && static_cast<int>(password->length()) >= 50;                            
  });

  auto components = Container::Vertical({
    input_username,
    input_password                                      
  });
  
  return Renderer(components, [=]{
    return vbox({

      vbox({
        separatorEmpty(),
        separatorEmpty(),

        text("██  ██ ███  ██ ██     ▄████▄ ▄█████ ██ ▄█▀   ██  ██ ▄████▄ ██  ██ ██    ██████") | color(theme::FONT_COLOR) | center, 
        text("██  ██ ██ ▀▄██ ██     ██  ██ ██     ████     ██▄▄██ ██▄▄██ ██  ██ ██      ██  ") | color(theme::FONT_COLOR) | center, 
        text("▀████▀ ██   ██ ██████ ▀████▀ ▀█████ ██ ▀█▄    ▀██▀  ██  ██ ▀████▀ ██████  ██  ") | color(theme::FONT_COLOR) | center, 

        separatorEmpty(),                                                              
        text("Enter master password to decrypt and open") | color(theme::FONT_COLOR) | center,
      }) | flex,
      vbox({
        text("USERNAME") | center | color(theme::FONT_COLOR) | bold,
        hbox({
          separatorEmpty() | xflex,
          input_username->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 60) | borderLight,
          separatorEmpty() | xflex,
        }) | xflex,

        separatorEmpty() | size(ftxui::HEIGHT, EQUAL, 3),

        text("MASTER PASSWORD") | center | color(theme::FONT_COLOR) | bold,
        hbox({
          separatorEmpty() | xflex,
          input_password->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 60) | borderLight,
          separatorEmpty() | xflex,
        }) | xflex,

        vbox({separatorEmpty()}) | flex,

        hbox({
          text("just imagine im a button") | color(theme::FONT_COLOR)
        }) | center,

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




