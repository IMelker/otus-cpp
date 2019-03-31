#define BOOST_TEST_MODULE bulk_test_module

#include <vector>
#include <iostream>
#include <list>
#include <tuple>

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../../7_task/radix_tree.h"

BOOST_AUTO_TEST_SUITE(nickname_test_suite)

BOOST_AUTO_TEST_CASE(radix_tree) {
  RadixTree<std::string, std::string> tree;

  BOOST_CHECK(tree.size() == 0);

  tree.insert(std::make_pair("test","test"));
  tree.emplace("testing","testing");
  tree["utest"] = "utest";
  BOOST_CHECK(tree.size() == 3);

  auto it_test = tree.find("test");
  auto it_testing = tree.find("testing");
  auto it_utest = tree.find("utest");
  BOOST_CHECK(it_test != tree.end() &&
              it_testing != tree.end() &&
              it_utest != tree.end());

  tree.erase("testing");
  tree.erase(it_utest);
  it_testing = tree.find("testing");
  it_utest = tree.find("utest");
  BOOST_CHECK(tree.size() == 1 &&
              it_testing == tree.end() &&
              it_utest == tree.end());

  auto prefix = tree.getPrefix(it_test);
  BOOST_CHECK(prefix == "t");
}

BOOST_AUTO_TEST_SUITE_END()
