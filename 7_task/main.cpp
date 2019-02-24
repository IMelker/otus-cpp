//
// Created by imelker on 2/7/19.
//

#include <iostream>
#include "radix_tree.h"

int main() {
  RadixTree<std::string, std::string> tree;

  std::string line;
  while (std::getline(std::cin, line)) {
    tree[line] = line;
  }

  std::cout << "Print RadixTree data with prefixes" << std::endl;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    std::cout << it->first << " " << tree.getPrefix(it) << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Print RadixTree structure" << std::endl;
  if (!tree.empty()) {
    std::cout << tree << std::endl;
  }

  return 0;
}