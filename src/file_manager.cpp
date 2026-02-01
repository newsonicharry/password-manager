#include "file_manager.h"
#include "constants.h"
#include <string_view>

namespace {
  constexpr std::string_view passwords_list_dir{"passwords_list"};
}

auto FileManager::get_directory_path() -> fs::path{
#ifdef __linux__
  const char* xdg_data{std::getenv("XDG_DATA_HOME")};

  if (xdg_data != nullptr){
    return fs::path(xdg_data) / constants::APP_NAME;
  }

  const char* home{std::getenv("HOME")};

  if (home != nullptr){
    return fs::path(xdg_data) / ".local" / "share" / constants::APP_NAME;
  }

  throw "XDG_DATA_HOME and HOME enviroment variables are not set";
  
#elif _WIN32

  const char* app_data{std::getenv("APPDATA")};

  if (app_data != nullptr){
    return fs::path(app_data) / constants::APP_NAME;
  }

  throw "APPDATA enviroment variable not set";

#else

  throw "You are on an unsupported platform";

#endif
  
};


auto FileManager::does_directory_exist() -> bool{
  return fs::is_directory(directory_path_);
}

void FileManager::create_directory(){
  fs::create_directory(directory_path_);
  fs::path passwords_directory{directory_path_ / passwords_list_dir};
}

auto FileManager::does_user_exist(std::string_view username) -> bool{
  return fs::exists(directory_path_ / passwords_list_dir / username);
}  


