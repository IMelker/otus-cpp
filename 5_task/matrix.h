//
// Created by imelker on 20.02.19.
//

#ifndef OTUS_CPP_VECTOR_MATRIX_H
#define OTUS_CPP_VECTOR_MATRIX_H

#include <vector>
#include <algorithm>
#include "helpers.h"

template <typename T, T DEFAULT, int N, int MAX>
class MatrixStep;
template <typename T, T DEFAULT, int MAX>
class MatrixElementView;

/**
 * @brief Main Matrix class for user interface
 */
template <typename T, T DEFAULT, int N = 2>
class Matrix {
  using MatrixStepN = MatrixStep<T, DEFAULT, N-1, N>;
  using KeyTuple = decltype(make_N_tuple<int, N>());
  using Data = std::vector<std::pair<KeyTuple, T>>;
 public:
  auto operator [](int idx) {
    auto key_tuple = make_N_tuple<int, N>();
    std::get<0>(key_tuple) = idx;
    return MatrixStepN(std::move(key_tuple), data_)[idx];
  }
  auto begin() {
    return data_.begin();
  }
  auto end() {
    return data_.end();
  }
  size_t size() const {
    return data_.size();
  }
 private:
  Data data_;
};
/**@{*/

/**
 * @brief Matrix subclass for bracket operator N depth
 */
template <typename T, T DEFAULT, int N, int MAX>
class MatrixStep {
  using MatrixStepN = MatrixStep<T, DEFAULT, N-1, MAX>;
  using KeyTuple = decltype(make_N_tuple<int, MAX>());
  using Data = std::vector<std::pair<KeyTuple, T>>;
 public:
  MatrixStep(KeyTuple&& tuple, Data& data)
      : key_tuple_(std::forward<KeyTuple>(tuple))
      , data_(data) {
  }

  auto operator [](int idx) {
    std::get<MAX-N-1>(key_tuple_) = idx;
    return MatrixStepN(std::move(key_tuple_), data_);
  }

 private:
  KeyTuple key_tuple_;
  Data& data_;
};

template<typename T, T DEFAULT, int MAX>
class MatrixStep<T,DEFAULT, 0, MAX> {
  using KeyTuple = decltype(make_N_tuple<int, MAX>());
  using Data = std::vector<std::pair<KeyTuple, T>>;
  using ElementView = MatrixElementView<T, DEFAULT, MAX>;
 public:
  MatrixStep(KeyTuple&& tuple, Data& data)
      : key_tuple_(std::forward<KeyTuple>(tuple))
      , data_(data) {
  }

  auto operator [](int idx) {
    std::get<MAX-1>(key_tuple_) = idx;
    return ElementView(std::move(key_tuple_), data_);
  }

 private:
  KeyTuple key_tuple_;
  Data& data_;
};
/**@{*/

/**
 * @brief Matrix element view data class
 */
template <typename T, T DEFAULT, int MAX>
class MatrixElementView {
  using KeyTuple = decltype(make_N_tuple<int, MAX>());
  using Data = std::vector<std::pair<KeyTuple, T>>;
  using DataIterator = typename Data::iterator;
 public:
  MatrixElementView(KeyTuple&& tuple, Data& data)
      : key_tuple_(std::forward<KeyTuple>(tuple))
      , data_(data)
      , iter_(FindIterator()) {
    LOG("Element view construction: ", __PRETTY_FUNCTION__);
  }

  ~MatrixElementView() = default;

  MatrixElementView& operator= (const T &value) {
    if (iter_ == data_.end()) {
      iter_ = FindIterator();
    }

    if (iter_ != data_.end()) {
      if (value == DEFAULT) {
        data_.erase(iter_);
        iter_ = data_.end();
      } else {
        iter_->second = value;
      }
    } else if (iter_ == data_.end() && value != DEFAULT) {
      iter_ = data_.emplace(data_.end(), key_tuple_, value);
    }
    return *this;
  }

  const T& value() const {
    if (iter_ == data_.end()) {
      iter_ = FindIterator();
    }
    if (iter_ == data_.end()) {
      return default_value;
    }
    return iter_->second;
  }

  operator const T&() const {
    return value();
  };

  template <typename U, U DEFAULT_, int MAX_>
  friend std::ostream& operator<<(std::ostream& stream, const MatrixElementView<U, DEFAULT_, MAX_>& element);

 private:
  DataIterator FindIterator() const {
    LOG("Another find iteration: ", __PRETTY_FUNCTION__);
    return std::find_if(data_.begin(), data_.end(),
        [this] (const auto& pair) {
          return (pair.first == key_tuple_);
        });
  }

  KeyTuple key_tuple_;
  Data& data_;
  mutable DataIterator iter_;
  static const T default_value = DEFAULT;
};

template <typename U, U DEFAULT_, int MAX_>
const U MatrixElementView<U, DEFAULT_, MAX_>::default_value;

template <typename U, U DEFAULT_, int MAX_>
std::ostream& operator<<(std::ostream& stream, const MatrixElementView<U, DEFAULT_, MAX_>& element) {
  return stream << element.value();
}

#endif //OTUS_CPP_VECTOR_MATRIX_H
