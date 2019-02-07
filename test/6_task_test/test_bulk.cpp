#define BOOST_TEST_MODULE bulk_test_module

#include <vector>
#include <iostream>
#include <list>
#include <tuple>

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../../6_task/bulk_composer.h"

BOOST_AUTO_TEST_SUITE(bulk_test_suite)

struct cout_redirect {
  explicit cout_redirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer)) {}

  ~cout_redirect() {
    std::cout.rdbuf(old);
  }

 private:
  std::streambuf *old;
};

BOOST_AUTO_TEST_CASE(bulk_different_size) {
  auto terminal_output = std::make_shared<BulkPrinter>();

  BulkComposer composer_1(1);
  BulkComposer composer_3(3);
  BulkComposer composer_12(12);

  terminal_output->subscribe(composer_1);
  terminal_output->subscribe(composer_3);
  terminal_output->subscribe(composer_12);

  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());
    for (int i = 1; i <= 12; ++i) {
      composer_1.appendCommand("cmd" + std::to_string(i));
    }
    for (int i = 1; i <= 12; ++i) {
      composer_3.appendCommand("cmd" + std::to_string(i));
    }
    for (int i = 1; i <= 12; ++i) {
      composer_12.appendCommand("cmd" + std::to_string(i));
    }
  }
  BOOST_CHECK(output.is_equal(
      "bulk: cmd1\n"
      "bulk: cmd2\n"
      "bulk: cmd3\n"
      "bulk: cmd4\n"
      "bulk: cmd5\n"
      "bulk: cmd6\n"
      "bulk: cmd7\n"
      "bulk: cmd8\n"
      "bulk: cmd9\n"
      "bulk: cmd10\n"
      "bulk: cmd11\n"
      "bulk: cmd12\n"
      "bulk: cmd1, cmd2, cmd3\n"
      "bulk: cmd4, cmd5, cmd6\n"
      "bulk: cmd7, cmd8, cmd9\n"
      "bulk: cmd10, cmd11, cmd12\n"
      "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9, cmd10, cmd11, cmd12\n"
  ));
}

BOOST_AUTO_TEST_CASE(bulk_brackets) {
  auto terminal_output = std::make_shared<BulkPrinter>();
  BulkComposer composer_3(3);
  terminal_output->subscribe(composer_3);

  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());
    composer_3.appendCommand("}");
    composer_3.appendCommand("cmd1");
    composer_3.appendCommand("cmd2");
    composer_3.appendCommand("cmd3");
    composer_3.appendCommand("{");
    composer_3.appendCommand("cmd4");
    composer_3.appendCommand("cmd5");
    composer_3.appendCommand("cmd6");
    composer_3.appendCommand("{");
    composer_3.appendCommand("cmd7");
    composer_3.appendCommand("cmd8");
    composer_3.appendCommand("}");
    composer_3.appendCommand("cmd9");
    composer_3.appendCommand("}");
    composer_3.appendCommand("{");
    composer_3.appendCommand("cmd10");
    composer_3.appendCommand("cmd11");
  }
  BOOST_CHECK(output.is_equal(
      "bulk: cmd1, cmd2, cmd3\n"
      "bulk: cmd4, cmd5, cmd6, cmd7, cmd8, cmd9\n"
  ));
}

BOOST_AUTO_TEST_SUITE_END()
