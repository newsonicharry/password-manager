#include "password_strength.h"
#include <array>
#include <cctype>
#include <cmath>
#include <cstddef>

using PasswordStrength = ui::password_strength::PasswordStrength;

namespace
{

constexpr int NUM_LOWER_CASE{26};
constexpr int NUM_UPPER_CASE{26};
constexpr int NUM_NUMBERS{10};
constexpr int NUM_SYMBOLS{32};

constexpr std::size_t num_strength_categories{ static_cast<std::size_t>(PasswordStrength::Size) };
constexpr std::array<int, num_strength_categories> min_entropy_by_strength{0,28,36,50,65};

 
}// unnamed namespace


//WARNING: does not account for more nuanced detections with weak human design flaws (such as repeated strings or common passwords)
auto ui::password_strength::classify_password_strength(std::string_view password) -> PasswordStrength
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

