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
#include <functional>
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
  
}// unnamed namespace


auto ui::screens::render_message_screen(state::AppState& app_state) -> ftxui::Component
{ 
  Component body{ui::components::render_contained_body(TEXT_X_AREA, TEXT_Y_AREA, [&]{ return render_internals(app_state);}) };
  Component header{ ui::components::render_header("MESSAGE")};
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
    
