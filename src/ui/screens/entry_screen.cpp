#include "screens.h"
#include "../theme.h"
#include "../components/container.h"
#include "../components/components.h"
#include "../components/input_field_component.h"
#include "../app_state.h"
#include "ui_constants.h"
#include "../../utils.h"
#include <cctype>
#include <cstddef>
#include <cstdint>
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
  Filter password_length_filter{filter_combiner(only_nums_input_filter, char_limit_input_filter(&app_state.entry.password_length, 2))}; 
  Component password_length_edit_field{create_input_field(&app_state.entry.password_length, "0", password_length_filter)};

  Component include_uppercase{Checkbox("Uppercase", &app_state.entry.include_uppercase) };
  Component include_lowercase{Checkbox("Lowercase", &app_state.entry.include_lowercase) };
  Component include_numbers{Checkbox("Numbers", &app_state.entry.include_numbers) };
  Component include_symbols{Checkbox("Symbols", &app_state.entry.include_symbols) };

  Component generate_password_button{create_button("GENERATE PASSWORD", [](){})};

  auto components = Container::Vertical({
    generate_password_button,

    Container::Horizontal({
      password_length_edit_field,
      include_uppercase,
      include_lowercase,
      include_numbers,
      include_symbols,
    })
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


void on_exit(state::AppState& app_state)
{
  app_state.entry.site = "";
  app_state.entry.username = "";
  app_state.entry.notes = "";
  app_state.entry.password = "";

  app_state.main_vault.populate(app_state.main_vault.vault.get());
  app_state.selected_screen = ui::state::SelectedScreen::MainVault; 
}

// save helpers

auto get_current_unix_time() -> int64_t
{
  const auto current_time = std::chrono::system_clock::now();
  const auto duration = current_time.time_since_epoch();
  int64_t unix_time = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

  return unix_time;
}

void on_save_edit(state::AppState& app_state, Vault* vault)
{
  const std::size_t index{ static_cast<std::size_t>(app_state.main_vault.entry_selected) };

  vault->modify_entry(index, MagicIdentifier::Site,     { app_state.entry.site });  
  vault->modify_entry(index, MagicIdentifier::Username, { app_state.entry.username });
  vault->modify_entry(index, MagicIdentifier::Note,     { app_state.entry.notes });
  vault->modify_entry(index, MagicIdentifier::Password, { app_state.entry.password });

  int64_t current_unix_time{ get_current_unix_time() };

  // puts the unix time into a secure buffer
  SecureBuffer unix_time_buffer{ sizeof(current_unix_time) };
  std::byte* write_ptr{ unix_time_buffer.get_write_ptr() };
  insert_into_ptr(write_ptr, current_unix_time); 

  vault->modify_entry(index, MagicIdentifier::DateModified, unix_time_buffer);
}

void on_save_new(state::AppState& app_state, Vault* vault)
{
  int64_t current_unix_time{ get_current_unix_time() };
  
  PasswordEntry new_password_entry{
    app_state.entry.site,
    app_state.entry.username,
    app_state.entry.password,
    app_state.entry.notes,
    current_unix_time,
    current_unix_time
  };

  vault->add_entry(std::move(new_password_entry));  
}

void on_save(state::AppState& app_state)
{
  Vault* vault{ app_state.main_vault.vault.get() };

  // creating a new entry
  if (app_state.entry.is_new)  { on_save_new(app_state, vault); }
  // modifing an existing entry
  else{ on_save_edit(app_state, vault); }

  try
  {
    vault->encrypt_to_file({ app_state.password });
  }
  catch(const Exception& exception)
  {
    on_exit(app_state);
    app_state.selected_screen = ui::state::SelectedScreen::Message; 

    app_state.message.message = exception.what(); 
    app_state.message.message_type = MessageType::Error; 
    app_state.message.next_screen = ui::state::SelectedScreen::MainVault; 
    app_state.message.title = Exception::TYPE_TO_STRING.at(static_cast<std::size_t>(exception.type())); 
    return;
  }

  on_exit(app_state);
}


auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components;

  auto build_edit_field{ [](std::string* edit_string, std::string_view field_name){
    Filter filter{filter_combiner(newline_input_filter, char_limit_input_filter(edit_string, constants::MAX_INPUT_CHARACTERS))};
    return create_input_field(edit_string, field_name, filter);
  }};
  
  Component site_edit_field    {build_edit_field(&app_state.entry.site,     "Edit site..."    )};
  Component username_edit_field{build_edit_field(&app_state.entry.username, "Edit username...")};
  Component password_edit_field   {build_edit_field(&app_state.entry.password, "Edit password...")};

  Component notes_edit_field{create_input_field(&app_state.entry.notes, "Edit notes...", char_limit_input_filter(&app_state.entry.notes, MAX_NOTES_LENGTH))};

  Component password_generation_component{ render_password_generation(app_state) };

  Component save_button{create_button("SAVE", [&]{ on_save(app_state); }, BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  Component cancel_button{create_button("CANCEL", [&]{ on_exit(app_state); }, BUTTON_WIDTH, BRIGHT_BUTTON_COLOR)};
  
  auto components = Container::Vertical({
    site_edit_field,
    username_edit_field,
    notes_edit_field,

    password_edit_field,
    password_generation_component,

    Container::Horizontal({
      save_button,
      cancel_button,
    })
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
            cancel_button->Render(),
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


