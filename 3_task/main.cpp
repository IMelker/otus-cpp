#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>

#include "my_std_allocator.h"

int main() {
  size_t n = 5;
  auto m = std::map<int, int, std::less<int>, MyStdAllocator<std::pair<const int, int>>>{};
  m.get_allocator().reserve(n);
  for (size_t i = 0; i < 4; ++i) {
      m[i] = i;
      std::cout << std::endl;
  }

  //for (auto i: m) {
  //    std::cout << "[" << i.first << "] = " << i.second << std::endl;
  //}
  std::cout << std::endl;
  return 0;
}