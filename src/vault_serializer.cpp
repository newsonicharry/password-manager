#include "vault_serializer.h"
#include "constants.h"
#include "crypto_engine.h"
#include "exception.h"
#include "password_entry.h"
#include "secure_buffer.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include <bitset>
#include <iostream>

using MagicIdentifier = protocol::MagicIdentifer;



constexpr std::size_t NUM_MAGIC_IDENTIFIER_BYTES{1};
constexpr std::size_t NUM_LENGTH_BYTES{2};
// the combined magic identifier and length bytes to prefix a piece of data within a password entry
constexpr std::size_t NUM_ENTRY_HEADER_BYTES{NUM_MAGIC_IDENTIFIER_BYTES + NUM_LENGTH_BYTES};



namespace
{

auto parse_entry(std::size_t& index, const SecureBuffer& vault_data) -> PasswordEntry
{
  Slices offsets{};

  if (static_cast<uint8_t>(vault_data[index]) != static_cast<uint8_t>(MagicIdentifier::Initial))
  {
    throw Exception("First byte did not start with an inital identifier.\n", Exception::ExceptionType::ParsingError);
  }
  index++;


  // a list of true and false of weather we have found a certain identifier 
  std::bitset<static_cast<std::size_t>(MagicIdentifier::Size)> identifiers_found{};
  // we can assume the first identifier exists as the if statment before confirmed its existance
  identifiers_found.set(static_cast<std::size_t>(MagicIdentifier::Initial));

  // we continue checking until we have run out of data or we have found all the identifiers
  while(index < vault_data.size() && !identifiers_found.all())
  {
    // make sure at least 3 bytes exist to parse the magic identifier and length
    const std::size_t remaining_bytes{vault_data.size() - index};
    if (remaining_bytes < NUM_ENTRY_HEADER_BYTES)
    {
      throw Exception("Insufficent bytes exist to parse entry.\n", Exception::ExceptionType::ParsingError);
    }

    // verifys the magic identifier
    uint8_t raw_magic_identifier{static_cast<uint8_t>(vault_data[index])};

    if (raw_magic_identifier >= static_cast<uint8_t>(MagicIdentifier::Size))
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

    // 
    // check if the entry length is less than or equal to the remaining message size  
    if ((index+length+NUM_ENTRY_HEADER_BYTES) > vault_data.size())
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


auto vault_serializer::parse_user_vault(const SecureBuffer& vault_data) -> std::vector<PasswordEntry>
{
  std::vector<PasswordEntry> entries{};

  int num_found_entries{0};
  std::size_t index{0};
  // loops while we either still have data left
  while (index < vault_data.size())
  {
    // parse entry updates the index for the next entr
    entries.push_back(parse_entry(index, vault_data));
    num_found_entries++;
  }

  return entries;
}


auto vault_serializer::generate_new_headers(const std::vector<PasswordEntry>& entries) -> FileHeaders
{
  const auto nonce{crypto_engine::generate_random_buffer<protocol::NUM_NONCE_BYTES>()};
  const auto salt{crypto_engine::generate_random_buffer<protocol::NUM_SALT_BYTES>()};

  const uint8_t iterations{protocol::DEFAULT_ITERATIONS};
  const uint16_t entry_count{static_cast<uint16_t>(entries.size())};

  uint64_t message_size{};
  for (const auto& entry : entries)
  {
    // the bytes to store just the real password entry data
    message_size += entry.get_num_bytes_stored();
    // the bytes to store the magic identifier and legnth for each password entry
    message_size += NUM_ENTRY_HEADER_BYTES * static_cast<uint64_t>(MagicIdentifier::Size); 
  }

  FileHeaders headers{nonce, salt, iterations, entry_count, message_size};
  
  return headers;
} 

