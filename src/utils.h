#pragma once

#include "password_entry.h"
#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <span>
#include <expected>

template <typename T>
void print_container(T& container, bool print_as_char = false)
{  
  std::cout << '[';
  for (std::size_t i{0}; i<std::size(container); i++)
  {
    
    if (print_as_char && std::isprint(static_cast<char>(container[i])))
    {
      std::cout << static_cast<char>(container[i]);
    }
    else{
      std::cout << static_cast<int>(container[i]);
    }

    if (i < std::size(container)-1)
    {
      std::cout << ',';
    }
    
  }
  std::cout << "]\n";
}


template <typename T>
void back_insert_vec(std::vector<std::byte>& vec, T&& data)
{
  if constexpr (std::is_integral_v<std::remove_reference_t<T>>)
  {
    const std::span integral_as_bytes{std::as_bytes(std::span{&data, 1})};
    std::copy(integral_as_bytes.begin(), integral_as_bytes.end(), std::back_inserter(vec));
  }
  else
  {
    std::transform(data.begin(), data.end(), std::back_inserter(vec), [](auto data){
      return static_cast<std::byte>(data);
    });
  }
}

// essential the back insert vec but with just a pointer
// the write pointer is an outparameter for the new location it ended at
template <typename T>
void insert_into_ptr(std::byte*& write_ptr, T&& data)
{
  if constexpr (std::is_integral_v<std::decay_t<T>>)
  {
    const std::span integral_as_bytes{std::as_bytes(std::span{&data, 1})};
    std::copy(integral_as_bytes.begin(), integral_as_bytes.end(), write_ptr);
    write_ptr += integral_as_bytes.size();
  }
  else
  {
    std::copy(std::bit_cast<std::byte*>(data.begin()), std::bit_cast<std::byte*>(data.end()), write_ptr);
    write_ptr += data.size();
  }


}
