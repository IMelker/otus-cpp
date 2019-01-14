#define BOOST_TEST_MODULE ip_filter_test_module

#include <vector>
#include <list>
#include <tuple>

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../../4_task/print_ip.h"

BOOST_AUTO_TEST_SUITE(ip_filter_test_suite)

struct cout_redirect {
  explicit cout_redirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer)) {}

  ~cout_redirect() {
    std::cout.rdbuf(old);
  }

 private:
  std::streambuf *old;
};

BOOST_AUTO_TEST_CASE(print_ip_as_integral_type) {
  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());
    PrintIp(char(-1));
    PrintIp(short(0));
    PrintIp(int(2130706433));
    PrintIp(long(8875824491850138409));
  }

  BOOST_CHECK(output.is_equal(
      "255\n"
      "0.0\n"
      "127.0.0.1\n"
      "123.45.67.89.101.112.131.41\n"
  ));
}

BOOST_AUTO_TEST_CASE(print_ip_as_string) {
  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());
    PrintIp(std::string("8.8.8.8"));
  }

  BOOST_CHECK(output.is_equal(
      "8.8.8.8\n"
  ));
}

BOOST_AUTO_TEST_CASE(print_ip_as_container) {
  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());
    PrintIp(std::vector<int>{255, 255, 255, 255});
    PrintIp(std::list<std::string>{"255", "255", "255", "255"});
  }

  BOOST_CHECK(output.is_equal(
      "255.255.255.255\n"
      "255.255.255.255\n"
  ));
}

BOOST_AUTO_TEST_CASE(print_ip_as_tuple) {
  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());
    PrintIp(std::make_tuple(127, 0, 0, 1));
    PrintIp(std::make_tuple("127", "0", "0", "1"));
  }

  BOOST_CHECK(output.is_equal(
      "127.0.0.1\n"
      "127.0.0.1\n"
  ));
}

BOOST_AUTO_TEST_SUITE_END()
