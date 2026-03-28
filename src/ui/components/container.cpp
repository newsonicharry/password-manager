#include "container.h"
#include "../theme.h"
#include <ftxui/component/component.hpp>

using namespace ftxui;
namespace theme = ui::theme;

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
 
auto ui::components::render_footer(std::string_view status_type, Color status_color) -> Component
{
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

