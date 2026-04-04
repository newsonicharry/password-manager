#pragma once

#include <cctype>
#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
#include <type_traits>
#include <vector>
#include <iostream>

namespace ui::components
{
    
  constexpr bool IS_PASSWORD_INPUT{true};

  using Filter = std::function<bool(const ftxui::Event&)>;

  const Filter no_input_filter = [](const ftxui::Event&){
    return false;
  };

  const Filter newline_input_filter = [](const ftxui::Event& event){
    return event.character() == "\n" || event.character() == "\r\n";
  };

  const Filter only_nums_input_filter = [](const ftxui::Event& event){
    // have to include these as some special characters are necessary for naviagtion or deletion
    bool is_backspace{ static_cast<int>(event.character()[0]) == 127 };
    bool is_arrow_key{ static_cast<int>(event.character()[0]) == 27 };
    bool is_digit{ static_cast<bool>(std::isdigit(event.character()[0])) };
    bool is_tab{ static_cast<int>(event.character()[0]) == 9 };

    return !(is_digit || is_backspace || is_arrow_key || is_tab);
  };

  inline auto char_limit_input_filter(std::string* input_field, std::size_t max_input_characters) -> Filter
  {
    return [=](const ftxui::Event& event){
      return event.is_character() && static_cast<int>(input_field->length()) >= max_input_characters;
    };
  };

  template <typename... Args>
  requires (std::same_as<std::decay_t<Args>, Filter> && ...)
  auto filter_combiner(Args&&... args) -> Filter {
    return [=](const ftxui::Event& event){
      return (... || args(event));
    };
  }

  
  auto create_input_field(std::string* input_field, std::string_view default_text, Filter input_rules = no_input_filter, bool is_password = false) -> ftxui::Component;

  constexpr int DEFAULT_BUTTON_WIDTH{0};
  constexpr bool BRIGHT_BUTTON_COLOR{true};
  auto create_button(std::string_view button_label, const std::function<void()>& caller, int button_width = 0, bool is_bright = false) -> ftxui::Component;
 
  auto create_strength_bar(std::size_t category_index, const std::vector<ftxui::Color>& color_by_index) -> ftxui::Element;
}
