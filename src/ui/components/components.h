#pragma once

#include <cctype>
#include <cstddef>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>

namespace ui::components
{
  constexpr int DEFAULT_BUTTON_WIDTH{0};
  constexpr bool BRIGHT_BUTTON_COLOR{true};
  auto create_button(std::string_view button_label, const std::function<void()>& caller, int button_width = 0, bool is_bright = false) -> ftxui::Component;
 
  auto create_strength_bar(std::size_t category_index, const std::vector<ftxui::Color>& color_by_index) -> ftxui::Element;
}
