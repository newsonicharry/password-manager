#include <iostream>
#include <istream>
#include <ostream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>


void drawScreen(){
  
  using namespace ftxui;


  std::string input_label;
  auto text_input = Input(&input_label, "placeholder"); 

  auto layout = Container::Vertical({
    text_input
  });

  auto renderer = Renderer(layout, [&]{
      return vbox({
                    text_input->Render()
                  });
    }
  );


  ScreenInteractive::Fullscreen().Loop(renderer);

}

auto main() -> int{

  constexpr int my_var{5};
  std::cout << my_var;

  
  return 0;
}



