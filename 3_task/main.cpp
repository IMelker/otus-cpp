#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include <memory>

#include "chunked_allocator.h"

template <typename T>
static void FillMap(T& map, int n) {
  for (int i = 0; i < n; ++i) {
    map[i] = i;
    std::cout << std::endl;
  }
}

template <typename T>
static void PrintMap(const T& map) {
  for (auto pair : map) {
    std::cout << "[" << pair.first << "] = " << pair.second << "\n";
  }
  std::cout << std::endl;
}

int main() {
  constexpr size_t n = 11;

  auto m = std::map<int, int, std::less<int>, ChunkedAllocator<std::pair<const int, int>, n>>{};
  FillMap(m,n);
  PrintMap(m);

  return 0;
}