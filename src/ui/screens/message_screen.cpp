#include "message_screen.h"
#include "../components/components.h"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;


namespace{

auto render_body() -> Component
{
  using namespace ui::components;

  Component acknowledge_button{create_button("", []{})};
  auto components = Container::Vertical({ acknowledge_button });
  
  return Renderer(components, [=]{
    return vbox({
       text("hi")           
    });                
  });
}

  
}// unnamed namespace


auto ui::screens::render_message_screen(std::string_view message, MessageType message_type, std::string_view title) -> ftxui::Component
{
  return render_body();
}
