#define BOOST_TEST_MODULE allocator_test_module

#include <boost/test/unit_test.hpp>

#include "../../3_task/chunked_allocator.h"
#include "../../3_task/s_list.h"

BOOST_AUTO_TEST_SUITE(allocator_test_suite)

constexpr size_t N = 10;
BOOST_AUTO_TEST_CASE(allocator_test_map) {
  std::map<int, long long, std::less<int>, ChunkedAllocator<long long, N>> map;

  for (int i = 0; i < 1000; ++i) {
    map[i] = i;
  }
  for (int i = 0; i < 1000; ++i) {
    BOOST_CHECK_EQUAL(map.at(i), i);
  }
}

BOOST_AUTO_TEST_CASE(allocator_test_s_list) {
  using List = SList<int, ChunkedAllocator<int, N>>;

  List list;

  for (int i = 0; i < 1000; ++i) {
    list.Add(i);
  }

  List::Iterator it(&list);
  int i = 0;
  while (!it.IsEnd()) {
    BOOST_CHECK_EQUAL(it->data, i);
    ++i;
    ++it;
  }
}

BOOST_AUTO_TEST_SUITE_END()
