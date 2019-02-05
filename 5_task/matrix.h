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

//template <typename T, T DEFAULT, int N>
//class MatrixIterator;

/**
* @brief  Helper for creating a tuple of N Elements
*/
template<size_t, class T>
using T_ = T;

template<class T, size_t... Is>
auto gen(std::index_sequence<Is...>) { return std::tuple<T_<Is, T>...>{}; }

template<class T, size_t N>
auto gen() { return gen<T>(std::make_index_sequence<N>{}); }



template<typename T, T DEFAULT, int N = 2>
struct Data {
  using FullTuple = decltype(gen<T, N>());
  using KeysTuple = decltype(gen<T, N-1>());

  Data() : data_value(T()) {}
  Data(const T &value) : data_value(value) {}

  void setKeys(KeysTuple tuple) { key_ = tuple;};

  operator Data&() {
    return *this;
  };
  operator const T&() const{
    return data_value;
  };

  Data& operator= (const T &value) {
    data_value = value;
    if (data_value != DEFAULT) {
      all_values.push_back(std::tuple_cat(key_, std::tie(value)));
    }
    return *this;
  }
  friend std::ostream& operator<<(std::ostream& out, const Data &data){
    out << data.data_value;
    return out;
  }

  KeysTuple key_;
  T data_value;
  static std::vector<FullTuple> all_values;
};

/**
 * @brief  Template matrix class with default values and depth
 */
 template <typename T, T DEFAULT, int N = 2, int Z = 1>
class Matrix {
  using MatrixData = Matrix<T, DEFAULT, N, Z + 1>;
  using KeysTuple = decltype(gen<T, N-1>());
 public:
  Matrix() : tuple_(KeysTuple()) { static_assert(N >= 1, "Matrix depth can't be less than 1"); }
  Matrix(KeysTuple tuple) : tuple_(std::forward<KeysTuple>(tuple)) {}

  MatrixData& operator [](int idx) {
    auto new_tuple = tuple_;
    std::get<Z-1>(new_tuple) = idx;

    auto data = MatrixData(std::move(new_tuple));
    return data_.insert(std::make_pair(idx, data)).first->second;
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
  KeysTuple tuple_;
  std::map<int, MatrixData> data_;
};

/**
 * @brief  Specialization for last Matrix class with max depth
 */
template <typename T, T DEFAULT, int N>
class Matrix<T, DEFAULT, N, N> {
  using data = Data<T, DEFAULT, N>;
  using KeysTuple = decltype(gen<T, N-1>());
 public:
  Matrix(KeysTuple tuple) : tuple_(std::forward<KeysTuple>(tuple)) {}
  data& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return data_[idx];
    }
    return data_[idx] = DEFAULT;
  }

  auto begin() {
    return data::all_values.begin();
  }

  auto end() {
    return data::all_values.end();
  }

  long size() const {
    return std::count_if(data_.begin(), data_.end(), [](auto i) {return i.second.data_value != DEFAULT;});
  }

 private:
  KeysTuple tuple_;
  std::map<int, data> data_;
};




/**
 * @brief  Iterator for matrix element's iteration
 */
/*
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
*/
#endif //OTUS_CPP_MATRIX_H
