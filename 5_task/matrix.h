//
// Created by imelker on 1/15/19.
//

#ifndef OTUS_CPP_MATRIX_H
#define OTUS_CPP_MATRIX_H

#include <map>
#include <tuple>
#include <vector>
#include <algorithm>
#include <iterator>

template <typename T, T DEFAULT, int N>
class MatrixIterator;

template<int...> struct index_tuple{};
/**
 * @brief  Template matrix class with default values and depth
 */
 template <typename T, T DEFAULT, int N = 2>
class Matrix {
  using MatrixData = Matrix<T, DEFAULT, N-1>;
  //using iterator = MatrixIterator<T, DEFAULT>;
 public:
  Matrix() { static_assert(N >= 1, "Matrix depth can't be less than 1"); }

  MatrixData& operator [](int idx) {
    return data_[idx];
  }

  auto begin() {
    return data_.begin()->second.begin();
  }

  auto end() {
    return data_.end()->second.end();
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

/**
 * @brief  Specialization for last Matrix class with max depth
 */
template <typename T, T DEFAULT>
class Matrix<T, DEFAULT, 1> {
  //using iterator = MatrixIterator<T, DEFAULT>;
 public:
  T& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return data_[idx];
    }
    return data_[idx] = DEFAULT;
  }

  auto begin() {
    return flat_data_.begin();
  }

  auto end() {
    return flat_data_.end();
  }

  long size() const {
    return std::count_if(data_.begin(), data_.end(), [](auto i) {return i.second != DEFAULT;});
  }

 private:
  std::map<int, T> data_;
};

/**
 * @brief  Helper for creating a tuple of N Elements
 */
template<size_t, class T>
using T_ = T;

template<class T, size_t... Is>
auto gen(std::index_sequence<Is...>) { return std::tuple<T_<Is, T>...>{}; }

template<class T, size_t N>
auto gen() { return gen<T>(std::make_index_sequence<N>{}); }


/**
 * @brief  Iterator for matrix element's iteration
 */

template <typename T, T DEFAULT, int N = 2>
class MatrixIterator: public std::iterator<std::input_iterator_tag, T>
{
  using matrix_tuple = decltype(gen<T, N>());
 private:
  MatrixIterator(T* p) : p_(p) {}
 public:
  MatrixIterator(const MatrixIterator &it) : p_(it.p_) { }

  static void addIterator(matrix_tuple& full_path_tuple) {
    ptr_v_.push_back(full_path_tuple);
  }

  bool operator!=(MatrixIterator const& other) const {
    return p_ != other.p;
  }

  typename MatrixIterator::reference operator*() const {
    return *p_;
  }

  MatrixIterator& operator++() {
    ++p_;
    return *this;
  }
 private:
  T* p_; // current
  static std::vector<matrix_tuple> ptr_v_;
};

#endif //OTUS_CPP_MATRIX_H
