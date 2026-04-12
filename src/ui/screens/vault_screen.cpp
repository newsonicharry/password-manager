#include "screens.h"
#include "../theme.h"
#include "../components/container.h"
#include "../app_state.h"
#include <cctype>
#include <ctime>
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/terminal.hpp>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

using namespace ftxui;
namespace theme = ui::theme;
namespace state = ui::state;

namespace {


template <typename... Args>
requires (std::same_as<std::decay_t<Args>, std::pair<std::string_view, std::string_view>> && ...)
auto section_builder(std::string_view window_name, Args&&... args) -> Element{

  auto data_builder{ [](std::string_view display_text, std::string_view value){
    return vbox({
      hbox({
        separatorEmpty() | size(ftxui::WIDTH, ftxui::EQUAL, 4),
        text(display_text) | bold | color(theme::FONT_COLOR),
        paragraph(value) | color(theme::FONT_COLOR)
      }),
         
    });
  }};

  std::vector<Element> internal_container{separatorEmpty()};
  (internal_container.push_back(data_builder(args.first, args.second)), ...);
  internal_container.push_back(separatorEmpty());

  return hbox({
    separatorEmpty() | size(ftxui::WIDTH, ftxui::EQUAL, 4),

    window(text(window_name),
      vbox(internal_container),
      BorderStyle::LIGHT
    ) | xflex,

    separatorEmpty() | size(ftxui::WIDTH, ftxui::EQUAL, 4),
  }) | xflex;
}



auto render_entry(state::AppState& app_state) -> Component
{
  using namespace std::string_view_literals;

  return Renderer([=, &app_state]{
    const PasswordEntry* password_entry{app_state.main_vault.current_entry};

    return vbox({
      separatorEmpty() | size(ftxui::HEIGHT, ftxui::EQUAL, 2),
    
      section_builder(
        "[ Account ]",
        std::pair{"Site        :    "sv, password_entry->get_site()},
        std::pair{"Username    :    "sv, password_entry->get_username()}
      ),

      separatorEmpty() | size(ftxui::HEIGHT, ftxui::EQUAL, 2),

      section_builder(
        "[ Notes ]",
        std::pair{"Notes       :    "sv, password_entry->get_note()}
      ),

      separatorEmpty() | size(ftxui::HEIGHT, ftxui::EQUAL, 2),

      section_builder(
        "[ Security ]",
        std::pair{"Password    :    "sv, password_entry->get_password()}
      ),

      separatorEmpty() | size(ftxui::HEIGHT, ftxui::EQUAL, 2),


      section_builder(
        "[ Metadata ]",
        // might be undefined behavior
        std::pair{"Created    :    "sv, static_cast<std::string_view>(std::format("{}", password_entry->get_date_created()))},
        std::pair{"Modified   :    "sv, static_cast<std::string_view>(std::format("{}", password_entry->get_date_modified()))}
      ),
    });
  });
}


auto render_sites(state::AppState& app_state) -> Component
{
  MenuOption option;

  option.on_change = [&]{
    Vault* vault = app_state.main_vault.vault.get();
    app_state.main_vault.current_entry = &vault->list_entries()[app_state.main_vault.entry_selected]; 
  };

  auto menu{Menu(&app_state.main_vault.sites, &app_state.main_vault.entry_selected, option) | color(theme::FONT_COLOR)};

  constexpr int USED_HEIGHT{6};
  auto usable_height{ [](){
    return Terminal::Size().dimy - USED_HEIGHT;
  }};

  return Renderer(menu, [=]{
    return menu->Render() | vscroll_indicator | frame | size(ftxui::HEIGHT, ftxui::LESS_THAN, usable_height());
  });
}


// footers

// helper
auto key_combo_builder(std::string_view key, std::string_view name) -> Element
{
  std::string key_text{"[" + std::string(key) + "] "};
    return hbox({
    separatorEmpty(),
    text(key_text) | color(theme::FONT_COLOR) | bold,
    text(name) | color(theme::FONT_COLOR),
    separatorEmpty(),
  }) | flex;
};


auto render_entry_footer() -> Component
{
  return Renderer([=]{
    return vbox({
      separator(),
      hbox({
        key_combo_builder("R", "REVEAL"),
        key_combo_builder("C", "COPY"),
        key_combo_builder("E", "EDIT"),
        key_combo_builder("D", "DELETE"),
      }) | xflex_grow,

    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

auto render_main_footer() -> Component
{ 
  return Renderer([=]{
    return vbox({
      separator(),
      hbox({
        key_combo_builder("/", "SEARCH"),
        key_combo_builder("N", "NEW ENTRY"),
        key_combo_builder("Q", "LOGOUT"),
      }) | xflex_grow,

    }) | bgcolor(theme::ALT_BG_COLOR); 
  });
}

auto render_body(state::AppState& app_state) -> Component
{
  using namespace ui::components;  
  // Component setup_button{create_button("CREATE VAULT", []{ std::cout << "Pressed"; }, MAX_BUTTON_WIDTH)};

  Component sites_side_bar{render_sites(app_state)};
  Component site_details{render_entry(app_state)};
  Component entry_footer{render_entry_footer()};
  
  auto components = Container::Vertical({
    sites_side_bar,
    site_details,
    entry_footer
  });
  
  return Renderer(components, [=, &app_state]{
    return vbox({
      hbox({
        vbox({
          text("VAULT") | hcenter | bold | color(theme::FONT_COLOR),
          separator(),
          sites_side_bar->OnRender(),
        }) | xflex_grow | size(ftxui::WIDTH, ftxui::EQUAL, Terminal::Size().dimx/2),
        separator() | hcenter,
        vbox({
          text("ENTRY DETAILS") | hcenter | bold | color(theme::FONT_COLOR), 
          separator(),
          site_details->Render(),
          separatorEmpty() | yflex_grow,
          entry_footer -> Render()
        }) | xflex_grow,
      }) | flex_grow,

                
   }) | flex_grow | bgcolor(theme::BODY_BG_COLOR);
  });
}


} // unnammed namespace


auto ui::screens::render_vault_screen(state::AppState& app_state) -> Component
{
  // add saving vault option
  auto on_key_press{[&](const Event& event){
    if (event == Event::Character('q'))
    {
      app_state.selected_screen = state::SelectedScreen::Quit;  
    }
    else if(event == Event::Character('/'))
    {
      app_state.selected_screen = state::SelectedScreen::Search;        
    }
    else if(event == Event::Character('d'))
    {
      app_state.selected_screen = state::SelectedScreen::Delete;        
    }
    // else if(event == Event::Character('n'))
    // {
      // app_state.selected_screen = state::SelectedScreen::;        
    // }

    

    return false;  
  }};

  
  Component footer{ render_main_footer() };
  Component body{ render_body(app_state) }; 
  Component layout { Container::Vertical({
    body,
    footer
  })};
  
  return Renderer(layout, [=]{
     return vbox({
       body->Render(),
       footer->Render()                  
     }) | borderLight | color(theme::BORDER_COLOR) | bgcolor(theme::BODY_BG_COLOR);
  }) | CatchEvent(on_key_press);
}

