#include "screens.h"
#include "../components/components.h"
#include "../components/container.h"
#include "../message_type.h"
#include "ui_constants.h"
#include <array>
#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>
#include <string_view>

using namespace ftxui;

namespace state = ui::state;

namespace{


constexpr int TEXT_X_AREA = 80;
constexpr int TEXT_Y_AREA = 22;
constexpr int BUTTON_WIDTH = TEXT_X_AREA - 20;


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
auto render_internals(state::AppState& app_state) -> Component
{

  using namespace ui::components;

  auto acknowledge_button{ create_button(
    "ACKNOWLEDGE",
    [&]{ app_state.selected_screen = app_state.message.next_screen; },
    ui::screens::constants::MAX_BUTTON_WIDTH,
    BRIGHT_BUTTON_COLOR
  )};

  auto layout { Container::Vertical({ acknowledge_button }) };

  return Renderer(layout, [=, &app_state]{
     
    return vbox({
      separatorEmpty() | yflex_grow,
      create_title_text( TITLE_TEXT_CONTAINER.at(static_cast<std::size_t>(app_state.message.message_type))),

      separatorEmpty() | yflex_grow,
      separatorEmpty() | yflex_grow,

      text(app_state.message.title) | center | yflex_grow | bold,
      separatorEmpty() | yflex_grow,
      text(app_state.message.message) | center | yflex_grow,

      separatorEmpty() | yflex_grow,
      separatorEmpty() | yflex_grow,
      
      acknowledge_button->Render(),
      separatorEmpty() | yflex_grow,
    });                  
  });
}

auto render_body(state::AppState& app_state) -> Component
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

  Component body_internal{ render_internals(app_state) };
  auto layout { Container::Vertical({ body_internal }) };
  
  return Renderer(layout, [=, &app_state]{
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


auto ui::screens::render_message_screen(ui::state::AppState& app_state) -> ftxui::Component
{ 
  
  Component header{ ui::components::render_header("MESSAGE")};
  Component body{ render_body(app_state) };
  Component layout { Container::Vertical({
    header,
    body,
  })};

  return Renderer(layout, [=, &app_state]{ 
    std::string_view message_type_string{ MESSAGE_TYPE_TO_STR.at(static_cast<std::size_t>(app_state.message.message_type)) };  
    Component footer{ ui::components::render_footer(message_type_string, theme::FONT_COLOR)};

   return vbox({
     header->Render(),
     body->Render(),
     footer->Render()                  
   }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);;
  });
}
    
