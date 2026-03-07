#include "vault.h"
#include "constants.h"
#include "crypto_engine.h"
#include "exception.h"
#include "password_entry.h"
#include "secure_buffer.h"
#include "vault_serializer.h"
#include <cassert>


void Vault::generate_dir_if_not_exists()
{
  if (!file_manager_.does_directory_exist())
  {
    file_manager_.create_directory();   
  }
}

auto Vault::list_entries() const ->  const std::vector<PasswordEntry>&
{
  return entries_;
}

void Vault::modify_entry(std::size_t index, protocol::MagicIdentifer identifier, const SecureBuffer& new_data)
{  
  // hate that assert only works on false
  // means i have to do fucking demorgans theorem in my head, i dont even remember junior year so idk if this is right
  assert(index < entries_.size() && "Index cannot be greater then the size of entires");
  assert(identifier != MagicIdentifier::Initial && identifier != MagicIdentifier::Size && "Identifier must be a valid identifier");

  entries_[index].modify(identifier, new_data);  
}


void Vault::try_decrypt_user_data(std::string_view username, const SecureBuffer& password)
{

  if (!file_manager_.does_user_exist(username))
  {
    throw Exception("Specified user does not exist.\n", Exception::ExceptionType::DecryptionError);
  }

  fs::path user_path{file_manager_.get_user_path(username)};

  SecureBuffer data{crypto_engine::decrypt_file(user_path, password)};

  entries_ = vault_serializer::parse_user_vault(data);
}

void Vault::try_encrypt_user_data() const
{

  
  // crypto_engine::encrypt_file(const EncryptionDataRefView &encryption_data, const FileHeaders &file_headers)
}


