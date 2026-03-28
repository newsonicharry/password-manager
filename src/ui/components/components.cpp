#include "components.h"
#include "../theme.h"
#include <cassert>
#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <vector>

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

auto ui::components::create_strength_bar(std::size_t category_index, const std::vector<ftxui::Color>& color_by_index) -> Element
{
  assert(category_index < color_by_index.size() && "Invalid index given; out of bounds");
  
  std::vector<Element> gauge_container{separatorEmpty()};
  for (std::size_t i{}; i < color_by_index.size(); i++)
  {
    Color bar_color{i <= category_index ? color_by_index[category_index] : Color::GrayLight};
    
    // we use a seperator character because a horizontal seperator next to a horizontal sepeartor is not possible
    // since it will use a vertical seperator if it sees that
    gauge_container.push_back(
      separatorCharacter("─") | color(bar_color) | bold | xflex_grow
    );
    gauge_container.push_back(separatorEmpty());
  }

  return hbox(gauge_container) | borderLight;
  
}
