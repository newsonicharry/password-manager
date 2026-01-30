#include "vault.h"
#include <filesystem>


// Vault::Vault(std::string_view username){
// }


// Todo: implmenent windows support
void Vault::generateDirectory(){
  namespace fs = std::filesystem;

#ifdef __linux__
  std::string directory_path{"~.local/password_manager"};
  if (!fs::exists(directory_path)){
    fs::create_directory(directory_path);
  } 


#elif _WIN32

#endif
}



