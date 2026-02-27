#include "vault_serializer.h"
#include "constants.h"
#include "exception.h"
#include "password_entry.h"
#include "secure_buffer.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include <bitset>

using MagicIdentifier = protocol::MagicIdentifer;



constexpr std::size_t NUM_MAGIC_IDENTIFIER_BYTES{1};
constexpr std::size_t NUM_LENGTH_BYTES{2};
// the combined magic identifier and length bytes to prefix a piece of data within a password entry
constexpr std::size_t NUM_ENTRY_HEADER_BYTES{NUM_MAGIC_IDENTIFIER_BYTES + NUM_LENGTH_BYTES};



namespace
{

auto parse_entry(std::size_t& index, const SecureBuffer& vault_data, const FileHeaders& file_headers) -> PasswordEntry
{
  Offsets offsets{};
  std::bitset<static_cast<std::size_t>(MagicIdentifier::Size)> identifiers_found;

  if (static_cast<uint8_t>(vault_data[0]) != static_cast<uint8_t>(MagicIdentifier::Initial))
  {
    throw Exception("First byte did not start with an inital identifier.\n", Exception::ExceptionType::ParsingError);
  }

  while(index < file_headers.message_size && !identifiers_found.all())
  {
    // make sure at least 3 bytes exist to parse the magic identifier and length
    const std::size_t remaining_bytes{vault_data.size() - index};
    if (remaining_bytes < NUM_ENTRY_HEADER_BYTES)
    {
      throw Exception("Insufficent bytes exist to parse entry.\n", Exception::ExceptionType::ParsingError);
    }

    // verifys the magic identifier
    uint8_t raw_magic_identifier{static_cast<uint8_t>(vault_data[index])};

    if (raw_magic_identifier >= static_cast<uint8_t>(MagicIdentifier::Size) || raw_magic_identifier == static_cast<uint8_t>(MagicIdentifier::Initial))
    {
      throw Exception("Magic identifier given is not a valid magic identifier.\n", Exception::ExceptionType::ParsingError);
    }

    if (identifiers_found.test(raw_magic_identifier))
    {
      throw Exception("Duplicated magic identifier found.\n", Exception::ExceptionType::ParsingError);
    }

    identifiers_found.set(raw_magic_identifier);    

    MagicIdentifier magic_identifier{static_cast<MagicIdentifier>(raw_magic_identifier)};

    // fetches entry length
    uint16_t length{};
    std::memcpy(&length, &vault_data[index+NUM_MAGIC_IDENTIFIER_BYTES], NUM_LENGTH_BYTES);

    // check if the entry length is less than or equal to the remaining message size  
    if ((remaining_bytes-NUM_ENTRY_HEADER_BYTES) < file_headers.message_size)
    {
      throw Exception("There is not enough bytes to parse from the length of the entry in vault entry.\n", Exception::ExceptionType::ParsingError);
    }

    index += NUM_ENTRY_HEADER_BYTES;
    offsets.set(magic_identifier, std::span{&vault_data[index], length});
    index += length;   

  }
    
  return PasswordEntry{vault_data, offsets};
}


}// unnammed namespace


auto vault_serializer::parse_user_vault(const SecureBuffer& vault_data, const FileHeaders& file_headers) -> std::vector<PasswordEntry>
{
  // this should not assert, though I don't wish to make that assumption
  assert(file_headers.message_size != vault_data.size() && "Message size and vault size should be equal");

  if (file_headers.message_size != vault_data.size())
  {
    throw Exception("Message size not equal to vault size.\n", Exception::ExceptionType::ParsingError);
  }

  std::vector<PasswordEntry> entries{};


  int num_found_entries{0};
  std::size_t index{0};
  while (index < file_headers.message_size && num_found_entries < file_headers.entry_count)
  {
    entries.push_back(parse_entry(index, vault_data, file_headers));
    num_found_entries++;
  }

  return entries;
}

