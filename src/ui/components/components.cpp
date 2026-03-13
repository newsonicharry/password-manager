#include "components.h"
#include "../theme.h"
#include <cstddef>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <functional>

using namespace ftxui;
namespace theme = ui::theme;


auto ui::components::create_input_field(std::string* input_field, std::string_view default_text, ui::components::Filter input_rules, bool is_password) -> ftxui::Component
{
  InputOption input_options;
  input_options.password = is_password;

  Component input = Input(input_field, default_text, input_options);

  input |= borderLight;
  input |= CatchEvent(input_rules);
  
  return input;
} 


auto ui::components::create_button(std::string_view button_label, const std::function<void()>& caller, int button_width) -> Component
{
  ButtonOption button_option;
  button_option.transform = [=](const EntryState& state){
    auto element = text(state.label) | center;

    if (state.focused)
    {
      element |=  borderLight | color(theme::BORDER_COLOR) | bold | inverted;
    }
    else
    {
      element |=  color(theme::FONT_COLOR) | borderLight | color(theme::BORDER_COLOR);
    }

    if (button_width != 0)
    {
      element |= size(ftxui::WIDTH, ftxui::EQUAL, button_width);   
    }

    return element;

  };
  Component button = Button(button_label, caller, button_option);
  button = button | center | bold;
  
  return button;
  
}


