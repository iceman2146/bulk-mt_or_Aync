#pragma once
#include <iostream>
#include <string>

int check(int c, const char **v) {
  if (c < 2) {
    std::cerr << "Надо: " << v[0] << "<N>" << std::endl;
    return 1;
  }
  long a_bulk_numbers{0};
  a_bulk_numbers = std::stoi(v[1]);
  if (a_bulk_numbers == 1||a_bulk_numbers < 0) {
    std::cerr << "N>0!" << std::endl;
    return 1;
  }

  return 0;
}