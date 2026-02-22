#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>

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
