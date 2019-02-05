//
// Created by imelker on 2/5/19.
//

#ifndef OTUS_CPP_MATRIX_H
#define OTUS_CPP_MATRIX_H

#include <map>
#include <vector>
#include <algorithm>
#include "helpers.h"

template <typename T, T DEFAULT, int N, int MAX>
class MatrixStep;
template <typename T, T DEFAULT, int MAX>
class MatrixElement;

/**
 * @brief Main Matrix class for user interface
 */
template <typename T, T DEFAULT, int N = 2>
class Matrix {
  using MatrixStepTDN = MatrixStep<T, DEFAULT, N-1, N>;
  using KeyTuple = decltype(make_N_tuple<T, N>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, N>*>>;
 public:
 /**
  * @brief Constructor with default MatrixStep for first row
  */
  Matrix() : data_(make_N_tuple<T, N>(), iterator_data_) {
  };

  MatrixStep<T,DEFAULT,N-2, N>& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return it->second;
    }
    auto key_tuple = make_N_tuple<T, N>();
    std::get<0>(key_tuple) = idx;

    auto step = MatrixStep<T,DEFAULT,N-2, N>(key_tuple, iterator_data_);
    auto emplace_it = data_.emplace(idx, step);
    return emplace_it.first->second;
  }

  auto begin() {
    return iterator_data_.begin();
  }

  auto end() {
    return iterator_data_.end();
  }

  size_t size() {
    return data_.size();
  }

 private:
  MatrixStepTDN data_;
  Iterators iterator_data_;
};

/**
 * @brief Matrix subclass for bracket operator N depth
 */
template<typename T, T DEFAULT, int N, int MAX>
class MatrixStep {
  using MatrixStepTDN = MatrixStep<T, DEFAULT, N-1, MAX>;
  using KeyTuple = decltype(make_N_tuple<T, MAX>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>>;
 public:
  MatrixStep(KeyTuple tuple, Iterators& iterator_data)
    : key_tuple_(std::move(tuple))
    , iterator_data_(iterator_data) {
  }

  MatrixStepTDN& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return it->second;
    }
    std::get<MAX-N-1>(key_tuple_) = idx;

    auto step = MatrixStepTDN(key_tuple_, iterator_data_);
    return data_.emplace(std::make_pair(idx, step)).first->second;
  }

  auto find(int idx) {
    return data_.find(idx);
  }

  auto end() {
    return data_.end();
  }

  template <class... Args>
  auto emplace(Args&&... args) {
    return data_.emplace(args...);
  }

  size_t size() const {
    size_t size_ = 0;
    for (auto& node : data_) {
      size_ += node.second.size();
    }
    return size_;
  }

 private:
  std::map<int, MatrixStepTDN> data_;
  KeyTuple key_tuple_;
  Iterators& iterator_data_;
};

/**
 * @brief Last step element in Matrix specialization
 */
template<typename T, T DEFAULT, int MAX>
class MatrixStep<T,DEFAULT, 0, MAX> {
  using KeyTuple = decltype(make_N_tuple<T, MAX>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>>;
 public:
  MatrixStep(KeyTuple tuple, Iterators& iterator_data)
      : key_tuple_(std::move(tuple))
      , iterator_data_(iterator_data) {
  }

  MatrixElement<T, DEFAULT, MAX>& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return it->second;
    }
    std::get<MAX-1>(key_tuple_) = idx;

    auto step = MatrixElement<T, DEFAULT, MAX>(key_tuple_, iterator_data_);
    return data_.emplace(std::make_pair(idx, step)).first->second;
  }

  long size() const {
    return std::count_if(data_.begin(), data_.end(), [](auto i) {return i.second.data_value_ != DEFAULT;});
  }

 private:
  std::map<int, MatrixElement<T, DEFAULT, MAX>> data_;
  KeyTuple key_tuple_;
  Iterators& iterator_data_;
};
/**@{*/

/**
 * @brief Matrix element data class
 */
template <typename T, T DEFAULT, int MAX>
class MatrixElement {
  using KeyTuple = decltype(make_N_tuple<T, MAX>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>>;
 public:
  MatrixElement(KeyTuple tuple, Iterators& iterator_data)
      : key_tuple_(tuple)
      , iterator_data_(iterator_data)
      , data_value_(DEFAULT) {
  }

  MatrixElement& operator= (const T &value) {
    data_value_ = value;
    auto it = std::find_if(iterator_data_.begin(), iterator_data_.end(),
        [&] (const std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>& pair) {
          return (pair.first == key_tuple_);
        });
    if (value == DEFAULT) {
      if (it != iterator_data_.end()) {
        iterator_data_.erase(it);
      }
      return *this;
    }
    if (it != iterator_data_.end()) {
      *it = std::make_pair(key_tuple_, this);
    } else {
      iterator_data_.emplace_back(key_tuple_, this);
    }
    return *this;
  }

  template <typename U, U DEFAULT_, int MAX_>
  friend std::ostream& operator<<(std::ostream& stream, const MatrixElement<U, DEFAULT_, MAX_>& element);

  T data_value_;
 private:
  KeyTuple key_tuple_;
  Iterators& iterator_data_;
};

template <typename U, U DEFAULT_, int MAX_>
std::ostream& operator<<(std::ostream& stream, const MatrixElement<U, DEFAULT_, MAX_>& element) {
  return stream << element.data_value_;
}

#endif //OTUS_CPP_MATRIX_H
