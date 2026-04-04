#include "screens.h"
#include "../theme.h"
#include "../components/container.h"
#include "../components/components.h"
#include "../app_state.h"
#include "ui_constants.h"
#include <cctype>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/terminal.hpp>
#include <string_view>


using namespace ftxui;

namespace theme = ui::theme;
namespace state = ui::state;
namespace constants = ui::screens::constants;

namespace {

constexpr int MAX_NOTES_LENGTH{256};
constexpr int BUTTON_WIDTH{ constants::STANDARD_WIDTH / 2  };
constexpr int NOTES_HEIGHT{ 6 };
constexpr int PASSWORD_LENGTH_WIDTH{ 5 };

// helper
auto render_password_generation(state::AppState& app_state) -> Component
{
  using namespace ui::components;

  // password generator
  // password length cant be more than 2 characters (max of 99)
  Filter password_length_filter{filter_combiner(only_nums_input_filter, char_limit_input_filter(&app_state.editor.password_length, 2))}; 
  Component password_length_edit_field{create_input_field(&app_state.editor.password_length, "0", password_length_filter)};

  Component include_uppercase{Checkbox("Uppercase", &app_state.editor.include_uppercase) };
  Component include_lowercase{Checkbox("Lowercase", &app_state.editor.include_lowercase) };
  Component include_numbers{Checkbox("Numbers", &app_state.editor.include_numbers) };
  Component include_symbols{Checkbox("Symbols", &app_state.editor.include_symbols) };

  Component generate_password_button{create_button("GENERATE PASSWORD", [](){})};

  auto components = Container::Vertical({
    generate_password_button,
    password_length_edit_field,
    
    include_uppercase,
    include_lowercase,
    include_numbers,
    include_symbols,
  });

  return Renderer(components, [=]{
    return vbox({
      hbox({
        separatorEmpty() | xflex,

        text("PASSWORD GENERATOR") | bold | vcenter ,       
        separatorEmpty() | xflex,
        separator(),
        separatorEmpty() | xflex,
        generate_password_button->Render(),

        separatorEmpty() | xflex,
      }),
      separatorEmpty(),

      hbox({
        text("Length ") | vcenter,
        password_length_edit_field->Render() | size(ftxui::WIDTH, ftxui::EQUAL, PASSWORD_LENGTH_WIDTH),
        separatorEmpty() | xflex,

        include_uppercase->Render() | vcenter,
        separatorEmpty() | xflex,
        include_lowercase->Render() | vcenter,
        separatorEmpty() | xflex,
        include_numbers->Render() | vcenter,
        separatorEmpty() | xflex,
        include_symbols->Render() | vcenter,
      }),
    }) | xflex | hcenter;
  }) | flex_grow;
}



auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components;

  auto build_edit_field{ [](std::string* edit_string, std::string_view field_name){
    Filter filter{filter_combiner(newline_input_filter, char_limit_input_filter(edit_string, constants::MAX_INPUT_CHARACTERS))};
    return create_input_field(edit_string, field_name, filter);
  }};
  
  Component site_edit_field    {build_edit_field(&app_state.editor.site,     "EDIT SITE"    )};
  Component username_edit_field{build_edit_field(&app_state.editor.username, "EDIT USERNAME")};
  Component email_edit_field   {build_edit_field(&app_state.editor.email,    "EDIT EMAIL"   )};
  Component password_edit_field   {build_edit_field(&app_state.editor.password, "EDIT PASSWORD"   )};

  Component notes_edit_field{create_input_field(&app_state.editor.notes, "EDIT NOTES", char_limit_input_filter(&app_state.editor.password_length, MAX_NOTES_LENGTH))};

  Component password_generation_component{ render_password_generation(app_state) };

  Component save_button{create_button("SAVE", [](){}, BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  Component exit_button{create_button("EXIT", [](){}, BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  
  auto components = Container::Vertical({
    site_edit_field,
    username_edit_field,
    email_edit_field,
    notes_edit_field,

    password_edit_field,
    password_generation_component,
    
    save_button,
    exit_button,
  });

  auto build_field_holder{ [](std::string_view field_name, const Component& field){
    return vbox({
      text(field_name) | bold | color(theme::FONT_COLOR) | hcenter,
      field->Render() | size(ftxui::WIDTH, ftxui::EQUAL, constants::MAX_INPUT_WIDTH),
    }) | xflex;
  }};
 
  return Renderer(components, [=]{
    return vbox({
      hbox({
        separatorEmpty() | xflex,

        vbox({
          separatorEmpty() | yflex,
          separatorEmpty() | yflex,

          build_field_holder("SITE", site_edit_field),
          build_field_holder("USERNAME", username_edit_field),
          build_field_holder("EMAIL", email_edit_field),

          separatorEmpty() | yflex,

          text("NOTES") | bold | color(theme::FONT_COLOR) | hcenter,
          notes_edit_field -> Render() | size(ftxui::WIDTH, ftxui::EQUAL, constants::MAX_INPUT_WIDTH) |
                                         size(ftxui::HEIGHT, ftxui::EQUAL, NOTES_HEIGHT),

          separatorEmpty() | yflex,
          build_field_holder("PASSWORD", password_edit_field),
          separatorEmpty(),
          password_generation_component->Render(),
        

          hbox({
            separatorEmpty() | xflex,
            save_button->Render(),
            separatorEmpty() | xflex,
            exit_button->Render(),
            separatorEmpty() | xflex,
          }),

          separatorEmpty() | yflex,
          separatorEmpty() | yflex,
        }) | xflex | hcenter,
        separatorEmpty() | xflex,

      }) | flex_grow
   }) | flex_grow | bgcolor(theme::BODY_BG_COLOR);
  });
}

} // unnammed namespace


auto ui::screens::render_entry_screen(state::AppState& app_state) -> Component
{
  Component header { components::render_header("NEW ENTRY") };
  Component footer{ components::render_footer("EDITING", Color::Purple) };
  Component body{ render_body(app_state) }; 

  Component layout { Container::Vertical({
    header,
    body,
    footer
  })};

  return Renderer(layout, [=]{
    return vbox({
      header->Render(),
      body->Render(),
      footer->Render()                  
    }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);
  });
}


