#pragma once

#include <cstddef>

namespace ui::screens::constants
{
  constexpr std::size_t STANDARD_WIDTH{60};

  constexpr std::size_t MAX_INPUT_CHARACTERS{50};
  constexpr std::size_t MAX_INPUT_WIDTH{STANDARD_WIDTH};
  constexpr std::size_t MAX_BUTTON_WIDTH{STANDARD_WIDTH};

  constexpr int NUM_RESERVED_Y_LINES = 6; // Repersents the 6 lines needed to display both the header and footer
  constexpr int NUM_RESERVED_X_LINES = 2; // Repersents the 2 lines needed to display the border on both sides
  
  
}
