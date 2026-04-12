#pragma once

#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <numeric>
#include <string_view>
#include <array>
#include <cstddef>
#include <ftxui/dom/elements.hpp>
#include <string>
#include "../theme.h"

namespace ui::components
{
  using namespace ftxui;
  namespace theme = ui::theme;

  auto render_header(std::string_view page_name) -> Component;

  auto render_footer(std::string_view status_type, Color status_color) -> Component;

  auto render_contained_body(int text_x_area, int text_y_area, const std::function<Component()>& internals) -> Component;
  
  template <std::size_t N>
  auto create_title_text(const std::array<std::string_view, N>& title_text) -> ftxui::Element
  {
    auto my_vbox = vbox({separatorEmpty()});

    std::string combined_title{ std::accumulate(title_text.begin(), title_text.end(), static_cast<std::string>(""), [](std::string accumulate, std::string_view section){
      accumulate += section;
      return accumulate + '\n';
    })};
      
    return vbox({
      paragraph(combined_title) | color(theme::FONT_COLOR) | center, 
    });
  }

}
