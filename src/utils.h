#pragma once

#include <array>
#include <cstddef>
#include <iostream>

template<typename T, std::size_t N>
void print_array(std::array<T, N>& array)
{
  std::cout << '[';
  for (std::size_t i{0}; i<array.size()-1; i++)
  {
    std::cout << static_cast<int>(array[i]) << ',';
  }
  std::cout << static_cast<int>(array[array.size()-1]) << "]\n";
}

