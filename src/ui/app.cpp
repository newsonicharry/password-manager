#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>
#include "app.h"
#include "app_state.h"
#include "screens/screens.h"

using namespace ftxui;


void ui::vault_renderer()
{
  state::AppState app_state{};
  app_state.initalize();
     
  ScreenInteractive screen = ScreenInteractive::Fullscreen();

  Component start_screen{ screens::render_start_screen(app_state) };
  Component login_screen{ screens::render_login_screen(app_state) };
  Component setup_screen{ screens::render_setup_screen(app_state) };
  Component message_screen{ screens::render_message_screen(app_state)};
  Component search_screen{ screens::render_search_screen(app_state)};
  Component delete_screen{ screens::render_delete_screen(app_state)};
  Component entry_screen{ screens::render_entry_screen(app_state)};
  Component vault_screen{ screens::render_vault_screen(app_state)};


  auto all_screens{ Container::Vertical({
    start_screen   | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Start;     }),
    login_screen   | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Login;     }),
    setup_screen   | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Setup;     }),
    message_screen | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Message;   }),
    search_screen  | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Search;    }),
    delete_screen  | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Delete;    }),
    entry_screen   | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::Entry;     }),
    vault_screen   | Maybe([&] { return app_state.selected_screen == state::SelectedScreen::MainVault; })
  })};

  
  auto root{ Renderer(all_screens, [&]{

      switch (app_state.selected_screen){

      case state::SelectedScreen::Login: return login_screen -> Render();
      case state::SelectedScreen::Setup: return setup_screen -> Render();
      case state::SelectedScreen::MainVault: return vault_screen -> Render();
      case state::SelectedScreen::Message: return message_screen -> Render();
      case state::SelectedScreen::Search: return search_screen -> Render();
      case state::SelectedScreen::Delete: return delete_screen -> Render();
      case state::SelectedScreen::Entry: return entry_screen -> Render();

      case state::SelectedScreen::Quit: screen.ExitLoopClosure()();
      default: return start_screen -> Render();
      }
    }
  )};

  screen.Loop(root);  
  app_state.destroy();
}



