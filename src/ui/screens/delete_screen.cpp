#include "screens.h"
#include "../components/components.h"
#include "../components/input_field_component.h"
#include "../components/container.h"
#include "ui_constants.h"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>
#include <functional>
#include <string_view>

using namespace ftxui;

namespace state = ui::state;
namespace constants = ui::screens::constants;

namespace{


constexpr int TEXT_X_AREA = 80;
constexpr int TEXT_Y_AREA = 22;


// helper
auto render_internals(state::AppState& app_state) -> Component
{
  using namespace ui::components;

  std::function<void()> on_update{ [&]{
    if (app_state.deleter.delete_field == "delete")
    {
      app_state.deleter.delete_field = "";
      app_state.selected_screen = ui::state::SelectedScreen::MainVault;
    } 
  }}; 

  Filter delete_filter{filter_combiner(newline_input_filter, char_limit_input_filter(&app_state.deleter.delete_field, constants::MAX_INPUT_CHARACTERS))};
  Component delete_field{ create_input_field(&app_state.deleter.delete_field, "Type 'delete' to delete...", delete_filter, !IS_PASSWORD_INPUT, on_update) };

  Component cancel_button{ create_button(
    "CANCEL",
    [&]{ app_state.selected_screen = ui::state::SelectedScreen::MainVault; },
    ui::screens::constants::MAX_BUTTON_WIDTH,
    BRIGHT_BUTTON_COLOR
  )};
  
  Component layout { Container::Vertical({
    delete_field,
    cancel_button,
  })};

  return Renderer(layout, [=, &app_state]{
     
    return vbox({
      separatorEmpty() | yflex_grow,

      text("Type 'delete' to delete this entry.") | center | yflex_grow | bold | color(theme::FONT_COLOR),
      separatorEmpty() | yflex_grow,
      delete_field->Render() | size(ftxui::WIDTH, ftxui::EQUAL, constants::MAX_INPUT_WIDTH) | center,
      separatorEmpty() | yflex_grow,
      cancel_button->Render(),

      separatorEmpty() | yflex_grow,      
    });                  
  });
}
  
}// unnamed namespace


auto ui::screens::render_delete_screen(state::AppState& app_state) -> ftxui::Component
{ 
  Component footer{ ui::components::render_footer("EDITING", Color::Purple)};
  Component body{ui::components::render_contained_body(TEXT_X_AREA, TEXT_Y_AREA, [&]{ return render_internals(app_state);}) };
  Component header{ ui::components::render_header("DELETE ENTRY")};

  Component layout { Container::Vertical({
    header,
    body,
    footer,
  })};

  return Renderer(layout, [=, &app_state]{ 
   return vbox({
     header->Render(),
     body->Render(),
     footer->Render()                  
   }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);;
  });
}
 
