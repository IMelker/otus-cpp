#define BOOST_TEST_MODULE matrix_test_module

#include <vector>
#include <list>
#include <tuple>

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../../5_task/matrix.h"

BOOST_AUTO_TEST_SUITE(matrix_test_suite)

BOOST_AUTO_TEST_CASE(matrix_minimum_required) {
  Matrix<int, 0> matrix;
  BOOST_CHECK(matrix.size() == 0);

  size_t count = 0;
  for (int i = 0; i <= 9; ++i) {
    matrix[i][i] = i;
    if(i) ++count;
  }
  for (int i = 9; i >= 0; --i) {
    matrix[i][9-i] = i;
    if(i) ++count;
  }
  BOOST_CHECK(matrix.size() == count);

  matrix[1][1] = 0;
  matrix[8][8] = 0;
  BOOST_CHECK(matrix.size() == (count-2));

  BOOST_CHECK(matrix[200][200] == 0);

  matrix[100][100] = 314;
  BOOST_CHECK(matrix[100][100] == 314);

  count = 0;
  for(auto it = matrix.begin(); it != matrix.end(); ++it) { ++count; }
  BOOST_CHECK(matrix.size() == count);
}

BOOST_AUTO_TEST_CASE(matrix_with_n_depth) {
  Matrix<int, 0, 20> matrix;
  BOOST_CHECK(matrix.size() == 0);

  size_t count = 0;
  for (int i = 0; i <= 9; ++i) {
    matrix[i][i][i][i][i][i][i][i][i][i][i][i][i][i][i][i][i][i][i][i] = i;
    if(i) ++count;
  }
  for (int i = 9; i >= 0; --i) {
    matrix[i][9-i][i][9-i][i][i][9-i][i][9-i][i][i][9-i][i][9-i][i][i][9-i][i][9-i][i] = i;
    if(i) ++count;
  }
  BOOST_CHECK(matrix.size() == count);

  matrix[1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1][1] = 0;
  matrix[8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8] = 0;
  BOOST_CHECK(matrix.size() == (count-2));

  BOOST_CHECK(matrix[2000][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8][8] == 0);

  (matrix[100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100] = 314) = 2;
  BOOST_CHECK(matrix[100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100][100] == 2);

  count = 0;
  for(auto it = matrix.begin(); it != matrix.end(); ++it) { ++count; }
  BOOST_CHECK(matrix.size() == count);
}

BOOST_AUTO_TEST_SUITE_END()
