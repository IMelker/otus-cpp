//
// Created by imelker on 2/7/19.
//

#include <iostream>
#include "radix_tree.h"

//Пользуясь полученной структурой определить минимальное количество
//символов достаточных для однозначного определения пользователя по его
//имени, предварительного добавленных в новый контейнер из стандартного
//ввода.

// Сортированная вставка элементов
// Опционально адаптировать алгоритм для работы с UTF-8
// Опционально связи между узлами отобразить графическими символами

int main(int argc, char *argv[]) {
  //RadixTree<std::string, std::string> tree; // будет хранить само себя <key, value>
  RadixTree<char* , std::string> tree;

  std::string line;
  while (std::getline(std::cin, line)) {
    tree[line.c_str()] = line;
    tree.insert(std::make_pair(line.c_str(), line));
    tree.emplace(line.c_str(), line);
  }

  tree.erase("test");
  tree.erase(tree.begin());

  std::cout << "Print RadixTree data with path" << std::endl;
  for (auto& pair: tree) {
    std::cout << pair.second << " " << tree.get_prefix(pair.first) << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Print RadixTree structure" << std::endl;
  if (!tree.empty()) {
    std::cout << tree.size() << std::endl;
    std::cout << tree << std::endl;
  }
  tree.clear();

  return 0;
}