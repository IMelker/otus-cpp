#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
#include <memory>

#include "chunked_allocator.h"
#include "s_list.h"

template <typename T>
static void FillMap(T& map, int n) {
  auto factorial = 1ll;
  for (int i = 0; i < n; ++i) {
    if (i == 0) {
      map[i] = i;
    } else {
      factorial *= i;
      map[i] = factorial;
    }
  }
}

template <typename T>
static void PrintMap(const T& map) {
  for (auto pair : map) {
    std::cout << pair.first << " " << pair.second << "\n";
  }
  std::cout << std::endl;
}

int main() {
  constexpr size_t N = 10;

  auto simple_map = std::map<int, long long>{};
  FillMap(simple_map,N);
  PrintMap(simple_map);

  auto map_with_alloc = std::map<int, long long, std::less<int>, ChunkedAllocator<std::pair<const std::string, std::string>, N>>{};
  FillMap(map_with_alloc,N);
  PrintMap(map_with_alloc);

  auto s_list = SList<int>{};
  for (int i = 0; i < 10; ++i) {
    s_list.Add(i);
  }
  std::cout << s_list << std::endl;

  auto s_list_with_alloc = SList<int, ChunkedAllocator<int, N>>{};
  for (int i = 0; i < 10; ++i) {
    s_list_with_alloc.Add(std::move(i));
  }
  std::cout << s_list_with_alloc;
  return 0;
}