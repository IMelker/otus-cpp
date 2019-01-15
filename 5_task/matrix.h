//
// Created by imelker on 1/15/19.
//

#ifndef OTUS_CPP_MATRIX_H
#define OTUS_CPP_MATRIX_H

#include <map>
#include <tuple>
#include <algorithm>
#include <iterator>

template <typename T, T DEFAULT, int N = 2>
class Matrix {
  using MatrixData = Matrix<T, DEFAULT, N-1>;
 public:
  Matrix() { static_assert(N >= 1, "Matrix depth can't be less than 1"); }

  MatrixData& operator [](int idx) {
    return data_[idx];
  }

  size_t size() {
    size_t size_ = 0;
    for (auto& node : data_) {
      size_ += node.second.size();
    }
    return size_;
  }
 private:
  std::map<int, MatrixData> data_;
};

template <typename T, T DEFAULT>
class Matrix<T, DEFAULT, 1> {
 public:
  T& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return data_[idx];
    }
    return data_[idx] = DEFAULT;
  }

  long size() const {
    return std::count_if(data_.begin(), data_.end(), [](auto i) {return i.second != DEFAULT;});
  }

 private:
  std::map<int, T> data_;
};

#endif //OTUS_CPP_MATRIX_H
