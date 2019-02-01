//
// Created by imelker on 1/15/19.
//

#include <cassert>
#include <iostream>
#include "matrix.h"

int main () {
  Matrix<int, 0> matrix;
  assert(matrix.size() == 0);

  // fill matrix
  for (int i = 0; i <= 9; ++i) {
    matrix[i][i] = i;
  }
  for (int i = 9; i >= 0; --i) {
    matrix[i][9-i] = i;
  }

  assert(matrix.size() == 18);

  //print matrix
  for (int i = 1; i <= 8; ++i) {
    for (int j = 1; j <= 8; ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "\n" << matrix.size() << std::endl;

  /*for(auto c: matrix) {
    std::cout << c;
    int x;
    int y;
    int v;
    std::tie(x, y, v) = c;
    std::cout << "x[" << x << "] y[" << y << "] v=" << v << std::endl;
  }*/

  return 0;
}