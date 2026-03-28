#pragma once

#include <ftxui/component/app.hpp>
#include <string_view>

namespace ui::screens
{

  enum class MessageType{
    Info,
    Success,
    Warning,
    Error,
  };
  auto render_message_screen(std::string_view message, MessageType message_type, std::string_view title) -> ftxui::Component;

}
