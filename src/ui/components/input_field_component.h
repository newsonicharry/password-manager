#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <functional>
#include <string>
#include <type_traits>


namespace ui::components
{
  // constants
  constexpr bool IS_PASSWORD_INPUT{true};

  // filters
  using Filter = std::function<bool(const ftxui::Event&)>;
  const Filter no_input_filter = [](const ftxui::Event&){ return false; };

  const Filter newline_input_filter = [](const ftxui::Event& event){
    return event == ftxui::Event::Return;
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

  // filter combiner
  template <typename... Args>
  requires (std::same_as<std::decay_t<Args>, Filter> && ...)
  inline auto filter_combiner(Args&&... args) -> Filter {
    return [=](const ftxui::Event& event){
      return (... || args(event));
    };
  }


  
  // template <typename... Args>
  inline auto create_input_field(std::string* input_field,
                          std::string_view default_text,
                          Filter input_rules = no_input_filter,
                          bool is_password = false
                          , std::function<void()> update_on_event = []{}
                        ) -> ftxui::Component
  {
    ftxui::InputOption input_options;
    input_options.password = is_password;
    input_options.on_change = [=]{ update_on_event(); };    
    
    ftxui::Component input = Input(input_field, default_text, input_options);

    input |= ftxui::borderLight;
    input |= ftxui::CatchEvent(input_rules);


    // input |= ftxui::CatchEvent(update_on_event);

    return input;
  }

  
}
