#include "message_screen.h"
#include "../components/components.h"
#include "../components/container.h"
#include <array>
#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>
#include <string_view>
#include <iostream>

using namespace ftxui;

namespace{


constexpr int TEXT_X_AREA = 120;
constexpr int TEXT_Y_AREA = 25;
constexpr int BUTTON_WIDTH = TEXT_X_AREA - 20;


using MessageType = ui::screens::MessageType;
constexpr std::array<std::string_view, static_cast<std::size_t>(MessageType::Size)> MESSAGE_TYPE_TO_STR{
  "INFO", "SUCCESS", "WARNING", "ERROR"
};

constexpr std::array< std::array<std::string_view, 3>, static_cast<std::size_t>(MessageType::Size) > TITLE_TEXT_CONTAINER
{{
  {{
    "██ ███  ██ ██████ ▄████▄", 
    "██ ██ ▀▄██ ██▄▄   ██  ██", 
    "██ ██   ██ ██     ▀████▀", 
  }},
  {{
    "▄█████ ██  ██ ▄█████ ▄█████ ██████ ▄█████ ▄█████", 
    "▀▀▀▄▄▄ ██  ██ ██     ██     ██▄▄   ▀▀▀▄▄▄ ▀▀▀▄▄▄", 
    "█████▀ ▀████▀ ▀█████ ▀█████ ██▄▄▄▄ █████▀ █████▀", 
  }},
  {{
    "██     ██ ▄████▄ █████▄  ███  ██ ██ ███  ██  ▄████ ", 
    "██ ▄█▄ ██ ██▄▄██ ██▄▄██▄ ██ ▀▄██ ██ ██ ▀▄██ ██  ▄▄▄", 
    " ▀██▀██▀  ██  ██ ██   ██ ██   ██ ██ ██   ██  ▀███▀ ", 
  }},
  {{
    "██████ █████▄  █████▄  ▄████▄ █████▄ ",
    "██▄▄   ██▄▄██▄ ██▄▄██▄ ██  ██ ██▄▄██▄",
    "██▄▄▄▄ ██   ██ ██   ██ ▀████▀ ██   ██",
  }}
}};



// helper
auto render_internals(std::string_view message, MessageType message_type, std::string_view title) -> Component
{

  using namespace ui::components;

  Component acknowledge_button{create_button("ACKNOWLEDGE", []{ std::cout << "i have been acknowledged"; }, BUTTON_WIDTH)};
  auto components { Container::Vertical({ acknowledge_button }) };

  Element title_text{ create_title_text( TITLE_TEXT_CONTAINER.at(static_cast<std::size_t>(message_type)) )};
  
  return Renderer(components, [=]{
    return vbox({
      separatorEmpty() | yflex_grow,
      create_title_text( TITLE_TEXT_CONTAINER.at(static_cast<std::size_t>(message_type))),

      separatorEmpty() | yflex_grow,
      separatorEmpty() | yflex_grow,

      text(title) | center | yflex_grow | bold,
      separatorEmpty() | yflex_grow,
      text(message) | center | yflex_grow,

      separatorEmpty() | yflex_grow,
      separatorEmpty() | yflex_grow,
      
      acknowledge_button->Render(),
      separatorEmpty() | yflex_grow,
    });                  
  });
}

auto render_body(std::string_view message, MessageType message_type, std::string_view title) -> Component
{
  // Repersents the 6 lines needed to display both the header and footer
  constexpr int NUM_RESERVED_Y_LINES = 6;

  // Repersents the 2 lines needed to display the border on both sides
  constexpr int NUM_RESERVED_X_LINES = 2;


  // Half of the remaining y area that can be free space (because there are are both a bottom and top empty area)
  // so anything that is left thats not used to fill the TEXT_Y_AREA
  auto calculate_y_section_area = [=](){
    return std::max((Terminal::Size().dimy - NUM_RESERVED_Y_LINES - TEXT_Y_AREA)/2, 0);
  };
  auto calculate_x_section_area = [=](){
      return std::max((Terminal::Size().dimx - NUM_RESERVED_X_LINES - TEXT_X_AREA)/2, 0);
  };

  Component body_internal{ render_internals(message, message_type, title) };
  auto components { Container::Vertical({ body_internal }) };
  

  return Renderer(components, [=]{
    return vbox({
        separatorEmpty() | yflex_grow | size(ftxui::HEIGHT, ftxui::EQUAL, calculate_y_section_area()),
  
        hbox({
          separatorEmpty() | xflex_grow | size(ftxui::WIDTH, ftxui::EQUAL, calculate_x_section_area()),

          body_internal->Render() | borderLight | xflex_grow,

          separatorEmpty() | xflex_grow | size(ftxui::WIDTH, ftxui::EQUAL, calculate_x_section_area()),
        }) | yflex_grow,

        separatorEmpty() | yflex_grow | size(ftxui::HEIGHT, ftxui::EQUAL, calculate_y_section_area()),
    }) | yflex_grow;                
  });
}

  
}// unnamed namespace


auto ui::screens::render_message_screen(std::string_view message, MessageType message_type, std::string_view title) -> ftxui::Component
{ 
  std::string_view message_type_string{ MESSAGE_TYPE_TO_STR.at(static_cast<std::size_t>(message_type)) };  
  
  Component header{ ui::components::render_header("MESSAGE")};
  Component footer{ ui::components::render_footer(message_type_string, theme::FONT_COLOR)};

  Component body{ render_body(message, message_type, title) };
  
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
