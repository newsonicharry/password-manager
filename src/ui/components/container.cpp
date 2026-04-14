#include "container.h"
#include "../screens/ui_constants.h"
#include "../theme.h"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <functional>
#include <string_view>

using namespace ftxui;
namespace theme = ui::theme;
namespace constants = ui::screens::constants;

auto ui::components::render_header(std::string_view page_name) -> Component
{
  return Renderer([=]{
    return vbox({
      hbox({
        text("VAULTTUI") | color(theme::ALT_FONT_COLOR) | bold,       
        text(" - ") | color(theme::FONT_COLOR),
        text(page_name) | color(theme::FONT_COLOR)       
      }),
      separator()
    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}


auto ui::components::render_contained_body(int text_x_area, int text_y_area, const std::function<Component()>& internals) -> Component
{
  // Half of the remaining y area that can be free space (because there are are both a bottom and top empty area)
  // so anything that is left thats not used to fill the TEXT_Y_AREA
  auto calculate_y_section_area = [=](){
    return std::max((Terminal::Size().dimy - constants::NUM_RESERVED_Y_LINES - text_y_area)/2, 0);
  };
  auto calculate_x_section_area = [=](){
      return std::max((Terminal::Size().dimx - constants::NUM_RESERVED_X_LINES - text_x_area)/2, 0);
  };

  Component body_internals{internals()};
  Component layout{Container::Vertical({body_internals})};
    
  return Renderer(layout, [=]{
    return vbox({
        separatorEmpty() | yflex_grow | size(ftxui::HEIGHT, ftxui::EQUAL, calculate_y_section_area()),

        hbox({
          separatorEmpty() | xflex_grow | size(ftxui::WIDTH, ftxui::EQUAL, calculate_x_section_area()),

          body_internals->Render() | borderLight | xflex_grow,

          separatorEmpty() | xflex_grow | size(ftxui::WIDTH, ftxui::EQUAL, calculate_x_section_area()),
        }) | yflex_grow,

        separatorEmpty() | yflex_grow | size(ftxui::HEIGHT, ftxui::EQUAL, calculate_y_section_area()),
    }) | yflex_grow;                
  });
     
}


 
auto ui::components::render_footer(std::string_view status_type, Color status_color) -> Component
{
  std::function<Element(std::string_view, std::string_view)> create_tab{ [](std::string_view input, std::string_view value){
    return hbox({
      separator(),           
      separatorEmpty(),
      text(input) | color(theme::FONT_COLOR) | bold,
      text(value) | color(theme::FONT_COLOR),
      separatorEmpty(),
    }); 
  }};
  
  return Renderer([=]{
    return vbox({
      separator(),
      hbox({
        hbox({
          separatorEmpty(),
          text("STATUS ") | color(theme::FONT_COLOR),
          text(status_type) | color(status_color) | bold,
          separatorEmpty(),
          separator(),           
        }),
        create_tab("[ENTER] ", "SELECT") | align_right | flex,
        create_tab("[←  ↑ →  ↓] ", "NEXT FIELD"),
        create_tab("[TAB] ", "NEXT FIELD"),
        create_tab("[↑ + TAB] ", "PREVIOUS FIELD"),

      }) | xflex_grow,

    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

