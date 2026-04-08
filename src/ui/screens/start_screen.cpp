#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "../components/container.h"
#include "../components/components.h"
#include "ui_constants.h"
#include "../theme.h"
#include "screens.h"
#include "../app_state.h"

using namespace ftxui;
namespace theme = ui::theme;
namespace constants = ui::screens::constants;
namespace state = ui::state;

namespace{


auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components;  
  
  Component login_button{ create_button("LOGIN", [&]{app_state.selected_screen = state::SelectedScreen::Login;}, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR) };
  Component setup_button{ create_button("SETUP", [&]{app_state.selected_screen = state::SelectedScreen::Setup;}, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR) };
  Component quit_button{ create_button("QUIT", [&]{app_state.selected_screen = state::SelectedScreen::Quit;}, constants::MAX_BUTTON_WIDTH, BRIGHT_BUTTON_COLOR) };

  auto components{ Container::Vertical({
    login_button,
    setup_button,
    quit_button
  })};
  
  return Renderer(components, [=]{
    return vbox({
      vbox({
        separatorEmpty(),
      }) | flex,      
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
      vbox({
        login_button->Render() | hcenter,  
        setup_button->Render() | hcenter,  
        quit_button->Render() | hcenter,  
      }),      
      vbox({
        separatorEmpty(),
      }) | flex
  
    }) | flex | bgcolor(theme::BODY_BG_COLOR); 
  });
}

  
}// unnamed namespace


auto ui::screens::render_start_screen(state::AppState& app_state) -> Component
{
  Component header{ components::render_header("V1.0.0 - SECURE PASSWORD MANAGER") };
  Component footer{ components::render_footer("LOCKED", theme::WARNING_FONT_COLOR)};

  Component body{ render_body(app_state) };

  Component layout { Container::Vertical({
    header,
    body,
    footer
  })};
 
  return Renderer (layout, [=] {
    return vbox({
      header->Render(),
      body->Render(),
      footer->Render(),
    }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);
  });
}
