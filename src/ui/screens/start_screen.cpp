#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/screen/screen.hpp>
#include "../theme.h"
#include "screens.h"

using namespace ftxui;
namespace theme = ui::theme;

namespace{

//helpers
auto render_footer() -> Component
{
  return Renderer([]{
    return vbox({
      separator(),
      hbox({
        separatorEmpty(),
        text("STATUS ") | color(theme::FONT_COLOR),
        text("LOCKED") | color(theme::WARNING_FONT_COLOR),
        separatorEmpty(),
        separator(),           
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
        text("V1.0.0 - SECURE PASSWORD MANAGER") | color(theme::FONT_COLOR)       
      }),
      separator()
    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

auto render_body() -> Component
{
  return  Renderer([]{
    return vbox({
      vbox({
        separatorEmpty(),
        separatorEmpty(),
      }),      
      vbox({
        text("██╗   ██╗ █████╗ ██╗   ██╗██╗  ████████╗") | color(theme::FONT_COLOR) | center,
        text("██║   ██║██╔══██╗██║   ██║██║  ╚══██╔══╝") | color(theme::FONT_COLOR) | center,
        text("██║   ██║███████║██║   ██║██║     ██║   ") | color(theme::FONT_COLOR) | center,
        text("╚██╗ ██╔╝██╔══██║██║   ██║██║     ██║   ") | color(theme::FONT_COLOR) | center,
        text(" ╚████╔╝ ██║  ██║╚██████╔╝███████╗██║   ") | color(theme::FONT_COLOR) | center,
        text("  ╚═══╝  ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝   ") | color(theme::FONT_COLOR) | center,         
        separatorEmpty()
      }) | flex,
      vbox({
        text("VAULT NOT UNLOCKED") | center | color(theme::FONT_COLOR) | bold,
        separatorEmpty(),
        separatorEmpty(),           
        text("Welcome to your password vault") | center | color(theme::FONT_COLOR),
        text("Start by logging in or setting up your personal vault") | center | color(theme::FONT_COLOR),    
        
      }) | flex,
      hbox({
        separatorEmpty(),
        text(" [ L ] LOGIN ") | color(theme::FONT_COLOR) | borderLight | center,
        separatorEmpty(),
        text(" [ S ] SETUP ") | color(theme::FONT_COLOR) | borderLight | center,
        separatorEmpty(),
        text(" [ Q ] QUIT ")  | color(theme::FONT_COLOR) | borderLight | center,        
        separatorEmpty(),
      }) | flex | center,
      vbox({
        separatorEmpty(),
        separatorEmpty(),      
      })
  
    }) | flex | bgcolor(theme::BODY_BG_COLOR); 
  });
}

  
}// unnamed namespace


auto ui::screens::render_start_screen() -> Component
{
  return Renderer ([] {
    return vbox({
      render_header()->Render(),
      render_body()->Render(),
      render_footer()->Render()            
    }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);
  });
}
