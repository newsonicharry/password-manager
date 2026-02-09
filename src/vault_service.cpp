#include "vault_service.h"
#include <string_view>


void VaultService::create_dir_if_not_exists()
{
  if (!file_manager_.does_directory_exist())
  {
    file_manager_.create_directory();  
  }
}

auto VaultService::try_get_user_data(std::string_view username) -> bool
{
  
    
  return true;
}
