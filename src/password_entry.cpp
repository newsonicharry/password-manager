#include "password_entry.h"
#include "constants.h"
#include "exception.h"
#include "secure_buffer.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <string_view>

// offset members

void Slices::set(MagicIdentifier identifier, std::span<const std::byte> value)
{
  switch (identifier) {
    case MagicIdentifier::Site:          site          = value; break;
    case MagicIdentifier::Username:      username      = value; break;
    case MagicIdentifier::Password:      password      = value; break;
    case MagicIdentifier::Note:          note          = value; break;
    case MagicIdentifier::DateCreated:   date_created  = value; break;
    case MagicIdentifier::DateModified:  date_modified = value; break;

    // its expected that the identifier has already been confirmed to be valid before hand
    default:
      throw Exception("Invalid identifier given.\n", Exception::ExceptionType::ParsingError);
  }   
}

auto Slices::get(MagicIdentifier identifier) const -> std::span<const std::byte>
{
  switch (identifier) {
    case MagicIdentifier::Site:         return site; 
    case MagicIdentifier::Username:     return username; 
    case MagicIdentifier::Password:     return password; 
    case MagicIdentifier::Note:         return note; 
    case MagicIdentifier::DateCreated:  return date_created; 
    case MagicIdentifier::DateModified: return date_modified; 
    // its expected that the identifier has already been confirmed to be valid before hand
    default:
      throw Exception("Invalid identifier given.\n", Exception::ExceptionType::ParsingError);
  }
}


auto Slices::get_num_bytes() const -> std::size_t {
 
  const std::size_t start_identifier{static_cast<std::size_t>(MagicIdentifier::Initial)+1};
  const std::size_t end_identifier{static_cast<std::size_t>(MagicIdentifier::Size)};

  std::size_t total_entry_bytes{0};
  
  for (std::size_t raw_identifier{start_identifier}; raw_identifier < end_identifier; raw_identifier++)
  {
    const MagicIdentifier identifier{static_cast<MagicIdentifier>(raw_identifier)};
    total_entry_bytes += get(identifier).size();
  }

  return total_entry_bytes;
}


// helpers

auto PasswordEntry::get_string_from_entry(std::span<const std::byte> entry) -> std::string_view
{
  return std::string_view {std::bit_cast<char*>(entry.data()), entry.size()};
}

auto PasswordEntry::get_date_from_entry(std::span<const std::byte> entry) -> std::chrono::sys_seconds
{
  
  std::time_t created_date{};
  std::memcpy(&created_date, entry.data(), entry.size()); 

  const std::chrono::sys_seconds time{std::chrono::seconds(created_date)};

  return time;
}

// member functions

auto PasswordEntry::get_site() const -> std::string_view
{
  return get_string_from_entry(entry_offset_.site);
}

auto PasswordEntry::get_username() const -> std::string_view
{
  return get_string_from_entry(entry_offset_.username);
}

auto PasswordEntry::get_password() const -> std::string_view
{
  return get_string_from_entry(entry_offset_.password);
}

auto PasswordEntry::get_note() const -> std::string_view
{
  return get_string_from_entry(entry_offset_.note);
}


auto PasswordEntry::get_date_created() const -> std::chrono::sys_seconds
{
  return get_date_from_entry(entry_offset_.date_created);
}

auto PasswordEntry::get_date_modified() const -> std::chrono::sys_seconds
{
  return get_date_from_entry(entry_offset_.date_modified);
}

// modifier
// todo: PLEASE HEAVILY TEST THIS FUNCTION, POINTER ARITHMETIC IS SCARY
void PasswordEntry::modify(MagicIdentifier identifier, const SecureBuffer& new_identifier_data)
{
  const auto slice{entry_offset_.get(identifier)};

  const std::size_t new_entry_size{entry_.size() - slice.size() + new_identifier_data.size()}; 

  SecureBuffer new_entry{new_entry_size};

  std::byte* write_ptr{new_entry.get_write_ptr()};

  //updates the data with in new secure_buffer

  // the number of bytes between the start of the entry and the potion of the entry holding the old identifier data
  std::ptrdiff_t num_start_elements{slice.data() - entry_.get_read_ptr()};

  std::copy(entry_.get_read_ptr(), entry_.get_read_ptr()+num_start_elements, write_ptr); 
  write_ptr += num_start_elements;

   
  std::copy(new_identifier_data.begin(), new_identifier_data.end(), write_ptr);

  write_ptr += new_identifier_data.size();
  
  const std::size_t written_elements{num_start_elements + new_identifier_data.size()};
  std::copy(entry_.get_read_ptr()+written_elements, entry_.get_read_ptr()+entry_.size(), write_ptr);

  // this is only to update the size of the new data
  // at this point every old slice is considered invalid and pointing to old memory
  // though techincally we have not yet moved the new memory in yet
  entry_offset_.set(identifier, std::span{std::bit_cast<std::byte*>(nullptr), new_identifier_data.size()});

  // updates the new pointers to the memory allocated in the new_entry
  const std::byte* pointer{new_entry.get_read_ptr()};
  for (std::size_t raw_identifier{protocol::start_identifier}; raw_identifier < protocol::end_identifier; raw_identifier++)
  {
    const MagicIdentifier identifier{static_cast<MagicIdentifier>(raw_identifier)};
    std::size_t identifier_size {entry_offset_.get(identifier).size()};

    entry_offset_.set(identifier, std::span{pointer, identifier_size});
    pointer += identifier_size;
    
  }
  
  entry_ = std::move(new_entry);
}


// constructor

PasswordEntry::PasswordEntry(const SecureBuffer& vault, const Slices& vault_offsets)
{
  SecureBuffer combined_entry_data{vault_offsets.get_num_bytes()};
  Slices local_slice{};

  const std::size_t start_identifier{static_cast<std::size_t>(MagicIdentifier::Initial)+1};
  const std::size_t end_identifier{static_cast<std::size_t>(MagicIdentifier::Size)};

  std::byte* write_ptr{combined_entry_data.get_write_ptr()};
  
  // loops through all identifiers
  for (std::size_t raw_identifier{start_identifier}; raw_identifier < end_identifier; raw_identifier++)
  {
    const MagicIdentifier identifier{static_cast<MagicIdentifier>(raw_identifier)};
    std::span<const std::byte> slice{vault_offsets.get(identifier)};

    // put the bytes from that identifier into my secure buffer
    std::copy(slice.begin(), slice.end(), write_ptr);
    // saves that data into my slice struct
    local_slice.set(identifier, std::span{write_ptr, slice.size()});
    
    // updates the pointer to the correct next spot in the vector
    write_ptr += slice.size();
  }

  entry_ = std::move(combined_entry_data);
  entry_offset_ = local_slice;  
}



