#include "vault.h"
#include "constants.h"
#include "crypto_engine.h"
#include "exception.h"
#include "password_entry.h"
#include "secure_buffer.h"
#include "utils.h"
#include "vault_serializer.h"
#include "converter.h"
#include <cassert>
#include <cstddef>
#include <expected>
#include <string_view>
#include <vector>


auto Vault::create_new(std::string_view username) -> std::expected<Vault, Exception>
{  
  Vault vault{username};

  if (vault.file_manager_.does_user_exist(username))
  {
    return std::unexpected(Exception("User already exists.\n", Exception::ExceptionType::VaultError));
  }

  // not necessary but nice its initalized
  FileHeaders new_headers{vault_serializer::generate_new_headers({})};
  vault.file_headers_ = new_headers;
  
  return vault;
}

auto Vault::open_existing(std::string_view username, const SecureBuffer& password) -> std::expected<Vault, Exception>
{
  Vault vault{username};

  if (!vault.file_manager_.does_user_exist(username))
  {
    return std::unexpected(Exception("Specified user does not exist.\n", Exception::ExceptionType::VaultError));
  }

  fs::path user_path{vault.file_manager_.get_user_path(username)};

  
  try
  {
    FileHeaders file_headers{vault_serializer::get_headers_from_path(user_path)};
    SecureBuffer data{crypto_engine::decrypt_file(user_path, file_headers, password)};
    vault.entries_ = vault_serializer::parse_user_vault(data);

    if (file_headers.entry_count != vault.entries_.size())
    {
      return std::unexpected(Exception("File header entry count and real entry count are not equal.\n", Exception::ExceptionType::VaultError));
    }

    vault.file_headers_ = file_headers;

  }
  catch (const Exception& exception)
  {
    return std::unexpected(exception);
  }


  return vault;
}


auto Vault::convert_from_bitwarden(std::string_view username, const fs::path& json_path) -> std::expected<Vault, Exception>
{
  Vault vault{username};

  if (vault.file_manager_.does_user_exist(username))
  {
    return std::unexpected(Exception("User already exists.\n", Exception::ExceptionType::VaultError));
  }
 
  try
  {
    SecureBuffer converted_data {convert_from_bitwarden_json(json_path)};
    vault.entries_ = vault_serializer::parse_user_vault(converted_data);
    
    vault.file_headers_ = vault_serializer::generate_new_headers(vault.entries_);
  }
  catch(const Exception& exception)
  {
    return std::unexpected(exception);
  }

  return vault;
}




void Vault::generate_dir_if_not_exists()
{
  if (!file_manager_.does_directory_exist())
  {
    file_manager_.create_directory();   
  }
}


void Vault::modify_entry(std::size_t index, protocol::MagicIdentifier identifier, const SecureBuffer& new_data)
{  
  assert(index < entries_.size() && "Index cannot be greater then the size of entires");
  assert(identifier != MagicIdentifier::Initial && identifier != MagicIdentifier::Size && "Identifier must be a valid identifier");

  entries_[index].modify(identifier, new_data);  
}


void Vault::encrypt_to_file(const SecureBuffer& password)
{
  file_headers_ = vault_serializer::generate_new_headers(entries_);


  const SecureBuffer entries_as_buffer{vault_serializer::convert_entries_to_buffer(entries_)};
  const fs::path file_path{file_manager_.get_user_path(username_)};

  std::vector<std::byte> encrypted_data{crypto_engine::encrypt_file(file_path, entries_as_buffer, file_headers_, password)};
  file_manager_.write_user_data(username_, encrypted_data);

}



