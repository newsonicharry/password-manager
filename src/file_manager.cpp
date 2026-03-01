#include "file_manager.h"
#include "constants.h"
#include "exception.h"
#include <filesystem>
#include <ios>
#include <string_view>
#include <sodium.h>
#include <fstream>

auto FileManager::find_directory_path() -> fs::path{
#ifdef __linux__

  const char* xdg_data{std::getenv("XDG_DATA_HOME")};

  if (xdg_data != nullptr){
    return fs::path(xdg_data) / project::APP_NAME;
  }
  
  const char* home{std::getenv("HOME")};


  if (home != nullptr){
    return fs::path(home) / ".local" / "share" / project::APP_NAME;
  }
  
  throw Exception("XDG_DATA_HOME and HOME enviroment variables are not set.\n", Exception::ExceptionType::FileError);
  
#elif _WIN32

  const char* app_data{std::getenv("APPDATA")};

  if (app_data != nullptr){
    return fs::path(app_data) / project::APP_NAME;
  }

  throw Exception("APPDATA enviroment variable not set.\n", Exception::ExceptionType::FileError);

#else

  throw Exception("You are on an unsupported platform.\n", Exception::ExceptionType::FileError);

#endif
  
};


auto FileManager::does_directory_exist() -> bool
{
  return fs::exists(directory_path_) && fs::is_directory(directory_path_);
}

void FileManager::create_directory()
{
  fs::create_directory(directory_path_);

  const fs::path passwords_directory{directory_path_ / project::PASSWORD_LIST_DIR};
  fs::create_directory(passwords_directory);
}


auto FileManager::does_user_exist(std::string_view username) const -> bool
{
  return fs::exists(get_user_path(username));
}  

void FileManager::delete_user(std::string_view username) const
{
  fs::remove(get_user_path(username));
}


auto FileManager::get_user_path(std::string_view username) const -> fs::path
{
  fs::path user_path {directory_path_ / project::PASSWORD_LIST_DIR / username};
  user_path.replace_extension(project::FILE_EXTENSION);
 
  return user_path;
}


void FileManager::write_user_data(std::string_view username, const std::vector<std::byte>& encrypted_data) const
{

  fs::path user_path{get_user_path(username)};

  fs::path temp_user_path{user_path};
  temp_user_path.replace_extension(project::FILE_EXTENSION_TEMP);

  fs::rename(user_path, temp_user_path);

  std::ofstream file{user_path};

  if (!file.is_open())
  {
    fs::rename(temp_user_path, user_path);
    throw Exception("Failed to create user passwords file.\n", Exception::ExceptionType::FileError);
  }

  file.write(std::bit_cast<char*>(encrypted_data.data()), static_cast<std::streamsize>(encrypted_data.size()));
  file.close();

  fs::remove(temp_user_path);
}


