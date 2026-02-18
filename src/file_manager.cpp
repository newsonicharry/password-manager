#include "file_manager.h"
#include "constants.h"
#include <filesystem>
#include <string_view>
#include <sodium.h>

namespace {
  constexpr std::string_view passwords_list_dir{"passwords_list"};
  constexpr std::string_view file_extension{".encrypted"};
}

auto FileManager::get_directory_path() -> fs::path{
#ifdef __linux__
  const char* xdg_data{std::getenv("XDG_DATA_HOME")};

  if (xdg_data != nullptr){
    return fs::path(xdg_data) / project::APP_NAME;
  }

  const char* home{std::getenv("HOME")};

  if (home != nullptr){
    return fs::path(xdg_data) / ".local" / "share" / project::APP_NAME;
  }

  throw "XDG_DATA_HOME and HOME enviroment variables are not set";
  
#elif _WIN32

  const char* app_data{std::getenv("APPDATA")};

  if (app_data != nullptr){
    return fs::path(app_data) / project::APP_NAME;
  }

  throw "APPDATA enviroment variable not set";

#else

  throw "You are on an unsupported platform";

#endif
  
};


auto FileManager::does_directory_exist() -> bool{
  return fs::is_directory(directory_path_) && fs::is_directory(directory_path_ / passwords_list_dir);
}

void FileManager::create_directory(){
  fs::create_directory(directory_path_);

  const fs::path passwords_directory{directory_path_ / passwords_list_dir};
  fs::create_directory(passwords_directory);
}

auto FileManager::does_user_exist(std::string_view username) -> bool{
  return fs::exists(directory_path_ / passwords_list_dir / username);
}  


void FileManager::decrypt_to_secure_buffer(SecureBuffer& secure_buffer, std::string_view username)
{
  
  if (!does_user_exist(username))
  {
    throw "Expected user file does not exist";
  }

  fs::path file_path{ directory_path_ / passwords_list_dir / username };
  file_path.replace_extension(file_extension);
  
  
    
}

