#include "vault.h"
#include <filesystem>


// Vault::Vault(std::string_view username){
  
// }


auto Vault::does_directory_exist() -> bool{
  return true;
}

// Todo: implmenent windows support
void Vault::generate_directory(){
  namespace fs = std::filesystem;

#ifdef __linux__
  std::string directory_path{"~.local/password_manager"};
  if (!fs::exists(directory_path)){
    fs::create_directory(directory_path);
  } 

#elif _WIN32

#endif
}
