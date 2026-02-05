#include "password_entry.h"
#include <cstddef>



// Storage:
// 	Each entry will contain in order:
// 	(432 bytes per entry)
// 	- 32 bytes for the site
// 	- 64 bytes for the username
// 	- 64 bytes for the password
// 	- 256 bytes for the notes
// 	- 8 bytes for the date created timestamp
// 	- 8 bytes for the date modified timestamp

namespace{
  constexpr std::size_t SITE_NAME_BYTES    { 32 };
  constexpr std::size_t USERNAME_BYTES     { 64 };
  constexpr std::size_t PASSWORD_BYTES     { 64 };
  constexpr std::size_t NOTES_BYTES        { 256 };
  constexpr std::size_t DATE_CREATED_BYTES { 8 };
  constexpr std::size_t DATE_MODIFIED_BYTES{ 8 };

  constexpr std::size_t SITE_NAME_OFFSET    { 0 };
  constexpr std::size_t USERNAME_OFFSET     { SITE_NAME_BYTES };
  constexpr std::size_t PASSWORD_OFFSET     { USERNAME_OFFSET+USERNAME_BYTES };
  constexpr std::size_t NOTES_OFFSET        { PASSWORD_OFFSET+PASSWORD_BYTES };
  constexpr std::size_t DATE_CREATED_OFFSET { NOTES_OFFSET+NOTES_BYTES };
  constexpr std::size_t DATE_MODIFIED_OFFSET{ DATE_CREATED_OFFSET+DATE_CREATED_BYTES };

  constexpr std::size_t SIZE_BYTES{ DATE_MODIFIED_OFFSET+DATE_MODIFIED_BYTES };
};






