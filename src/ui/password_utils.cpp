#include "password_utils.h"
#include "../exception.h"
#include "../utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <functional>
#include <sodium/randombytes.h>
#include <vector>

using PasswordStrength = ui::password_utils::PasswordStrength;


constexpr int NUM_LOWER_CASE{26};
constexpr int NUM_UPPER_CASE{26};
constexpr int NUM_NUMBERS{10};
constexpr int NUM_SYMBOLS{32};

constexpr std::size_t NUM_CHAR_TYPE{4};

constexpr std::size_t num_strength_categories{ static_cast<std::size_t>(PasswordStrength::Size) };
constexpr std::array<int, num_strength_categories> min_entropy_by_strength{0,28,36,50,65};


namespace{
// password generation helpers

// picks the nth char out of all ascii chars that fit the functions criteria
// we keep item_index as an index instead of a count because externally it should appear as if we are picking from a pool of existing characters 
auto select_random_char(std::size_t item_index, const std::function<int(char)>& is_valid_char) -> char
{
  int num_found_chars{};

  // looping through all ascii values (0 -> 127)
  constexpr int NUM_BITS{ 7 };
  for (std::size_t i{}; i < static_cast<std::size_t>(pow(2, NUM_BITS)); i++)
  {
    char curr_char{ static_cast<char>(i) };
    if (static_cast<bool>(is_valid_char(curr_char)))
    {
      num_found_chars++;
    }

    // negative values are not an issue here
    if ( (num_found_chars-1) == item_index )
    {
      return curr_char;
    }
  }

  assert(false && " Item index is out of bounds, character pool is therefore incorrect.");
}

  
}; // unnamed namespace


auto ui::password_utils::generate_random_password(std::size_t length, std::bitset<4> options) -> SecureBuffer
{
  // so that we're always guaranteed at least one of the characters type even if all 4 are selected
  if (length < NUM_CHAR_TYPE)
  {
    throw Exception("Password length must be at least 4 characters.\n", Exception::ExceptionType::UiError);
  }
  
  const std::size_t num_options{ options.count() };
  
  if (num_options == 0)
  {
    throw Exception("One option must be selected.\n", Exception::ExceptionType::UiError);
  }

  const std::size_t characters_per_type{ length / num_options };
  int excess_characters{ static_cast<int>(length % num_options) };
  

  std::vector<std::size_t> num_chars_by_type(NUM_CHAR_TYPE);
  std::vector<std::size_t> available_char_types{};
  
  for (std::size_t i{0}; i < options.size(); i++)
  {
    if (options.test(i))
    {
      // sets each corresponding index of the set character to the amount of characters that will be of that character type
      num_chars_by_type[i] = (excess_characters == 0) ? characters_per_type : characters_per_type + std::min(1, excess_characters--);
      available_char_types.push_back(i);
    }
    
  }

  // creates a vector of length password length, where each index of the vector refers to the type of char that will be there in the generated password
  std::vector<std::size_t> char_type_by_index{};
  for (std::size_t i{0}; i < length; i++)
  {
    const std::size_t available_char_type_index{ randombytes_uniform(available_char_types.size()) };  
        
    const std::size_t selected_char_type{ available_char_types[available_char_type_index] };
    const std::size_t chars_left_of_type{ num_chars_by_type[available_char_type_index] };
      
    num_chars_by_type[available_char_type_index] -= 1;

    // get rid of char types that we have already fully filled and met the number as required by the num_chars_by_type
    // we use 1 instead of 0 because although the indexed value is updated the original variable has not been
    if (chars_left_of_type == 1)
    {
      available_char_types.erase(available_char_types.begin() + available_char_type_index);
      num_chars_by_type.erase(num_chars_by_type.begin() + available_char_type_index);
    }

    char_type_by_index.push_back(selected_char_type);
  }

  // functions that determine if a char meets the criteria for a specified bucket
  // Example: uppercase characters would use std::isupper
  std::array<std::function<int(char)>, NUM_CHAR_TYPE> bucket_funcitons{
    static_cast<int(*)(int)>(std::isupper),
    static_cast<int(*)(int)>(std::islower),
    static_cast<int(*)(int)>(std::isdigit),
    static_cast<int(*)(int)>(std::ispunct),
  };

  std::array<std::size_t, NUM_CHAR_TYPE> pool_sizes{
    static_cast<std::size_t>(NUM_UPPER_CASE),
    static_cast<std::size_t>(NUM_LOWER_CASE),
    static_cast<std::size_t>(NUM_NUMBERS),
    static_cast<std::size_t>(NUM_SYMBOLS)
  };

  SecureBuffer final_password{length};
  std::byte* write_ptr{ final_password.get_write_ptr() };

  for (std::size_t i{0}; i < length; i++)
  {
    std::size_t char_type{ char_type_by_index[i] };
    std::size_t char_type_index{ randombytes_uniform(pool_sizes.at(char_type)) };
    std::function<int(char)> validator_function{ bucket_funcitons.at(char_type) };

    char selected_char{ select_random_char(char_type_index, validator_function)};    
    insert_into_ptr(write_ptr, selected_char);
  }
  
  print_container(final_password, true);
  return final_password;
}


//WARNING: does not account for more nuanced detections with weak human design flaws (such as repeated strings or common passwords)
auto ui::password_utils::classify_password_strength(const std::string& password) -> PasswordStrength
{
  bool found_lowercase{false};
  bool found_uppercase{false};
  bool found_number{false};
  bool found_symbol{false};

  for (const auto& character : password)
  {     
    if (static_cast<bool>(std::islower(character))) found_lowercase = true;
    if (static_cast<bool>(std::isupper(character))) found_uppercase = true;
    if (static_cast<bool>(std::isdigit(character))) found_number = true;
    if (static_cast<bool>(std::ispunct(character))) found_symbol = true;
    
  }

  // how many possible characters based off the character sets used
  const int pool_size{ static_cast<int>(found_lowercase) * NUM_LOWER_CASE +
                       static_cast<int>(found_uppercase) * NUM_UPPER_CASE +
                       static_cast<int>(found_number)    * NUM_NUMBERS +
                       static_cast<int>(found_symbol)    * NUM_SYMBOLS};


  // H = L*log2(P), where H is entropy, L is length and P is the pool size
  const int entropy_bits{ static_cast<int>(static_cast<double>(password.length()) * std::log2(pool_size))};


  // finds the password strength based off its entropy, see the min_entropy_by_strength for required entropy bits
  PasswordStrength password_strength{ PasswordStrength::VeryWeak  };
  for (std::size_t raw_password_strength{}; raw_password_strength<num_strength_categories; raw_password_strength++)
  {
    int required_entroy_bits{ min_entropy_by_strength.at(raw_password_strength) };

    if (required_entroy_bits > entropy_bits )
    {
      return password_strength;
    }

    password_strength = static_cast<PasswordStrength>(raw_password_strength);
    
  }

  
  return password_strength;
}

