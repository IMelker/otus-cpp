//
// Created by imelker on 2/7/19.
//

#include <cassert>
#include <cctype>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

#include "executor.h"
#include "bulk_composer.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Failed: Wrong number of arg_parser" << std::endl;
    exit(1);
  }

  int bulk_size = 0;
  try {
    char* ptr_bulk_size = argv[1];
    bool is_digit = std::all_of(ptr_bulk_size,
                                ptr_bulk_size + strlen(ptr_bulk_size),
                                [] (auto c) { return isdigit(c); });
    if (is_digit) {
      bulk_size = std::stoi(argv[1]);
    } else
      throw std::exception();

    if (bulk_size <= 0)
      throw std::exception();
  } catch (...) {
    std::cout << "Failed: Wrong bulk_size param: " << argv[1] << std::endl;
    exit(1);
  }

  BulkComposer composer(bulk_size);

  // std::shared_ptr here is due to observed behavior and using std::enable_shared_from_this
  auto terminal_output = std::make_shared<BulkPrinter>();
  auto file_output = std::make_shared<BulkWriter>();
  terminal_output->subscribe(composer);
  file_output->subscribe(composer);

  std::string line;
  while (std::getline(std::cin, line)) {
    composer.appendCommand(line);
  }

  return 0;
}