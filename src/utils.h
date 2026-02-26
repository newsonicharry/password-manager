#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include <span>

template<typename T>
void print_container(T& container, bool print_as_char = false)
{
  std::cout << '[';
  for (std::size_t i{0}; i<std::size(container); i++)
  {
    if (print_as_char)
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



template<typename T>
void back_insert_vec(std::vector<std::byte>& vec, T&& data)
{
  if constexpr (std::is_integral_v<std::remove_reference_t<T>>)
 {
    const std::span integral_as_bytes{std::as_bytes(std::span{&data, 1})};
    std::copy(integral_as_bytes.begin(), integral_as_bytes.end(), std::back_inserter(vec));
  }
  else if constexpr(std::is_same_v<std::decay_t<T>, std::string>){
    std::transform(data.begin(), data.end(), std::back_inserter(vec), [](char character){return static_cast<std::byte>(character);});
  }
  else if constexpr(std::is_reference_v<T>)
  {
    std::copy(data.begin(), data.end(), std::back_inserter(vec));
  }
}

