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

/*
 * А что за затея со списком? Максимум что могу со списком придумать, это когда вы оператором []
 * не столько по матрице ходите, сколько вектор/тьюпл координат собираете (у вас он даже есть key_tuple_)
 * и в самый последний момент с этим тьюплом можно искать значение, но не в списке, а в мапе или хэштаблице.
 * Смущает удаление элемента при присвоении в дефолт. Если я правильно понял затею, то удаление в
 * MatrixElement::operator=() может смело удалить элемент из ближайшей мапы, а дальше по цепочке вверх лишь
 * проверить, что мапа не опустела и не стоит ли ее тоже удалить. Сейчас получается, что мы утратили это
 * знание и делаем удаление с чистого листа.
 *
 * Итого - у вас почти все есть, чтобы сделать плоское хранение матрицы, с примитивным итератором и удалением.
 * Попробуем?
 */

/**
 * @brief helper recoursive erase method
 */
template<typename T, T DEFAULT, int N, int MAX>
void RecursiveErase(MatrixStep<T, DEFAULT, N, MAX>& data, const decltype(make_N_tuple<int, MAX>())& tuple) {
  auto idx = std::get<MAX-N-1>(tuple);
  auto it = data.find(idx);
  if (it != data.end()) {
    RecursiveErase(it->second, tuple);
    if (it->second.size() == 0) {
      data.erase(idx);
    }
  }
}

template <typename T, T DEFAULT, int MAX>
void RecursiveErase(MatrixElement<T, DEFAULT, MAX>&, const decltype(make_N_tuple<int, MAX>())&) {
  return;
}
/**@{*/

/**
 * @brief Main Matrix class for user interface
 */
template <typename T, T DEFAULT, int N = 2>
class Matrix {
  using MatrixStepTDN = MatrixStep<T, DEFAULT, N-1, N>;
  using KeyTuple = decltype(make_N_tuple<int, N>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, N>*>>;
 public:
 /**
  * @brief Constructor with default MatrixStep for first row
  */
  Matrix() : data_(make_N_tuple<int, N>(), iterator_data_, this) {
  };
  ~Matrix() = default;

  MatrixStep<T,DEFAULT,N-2, N>& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return it->second;
    }
    auto key_tuple = make_N_tuple<int, N>();
    std::get<0>(key_tuple) = idx;
    return data_.emplace(idx,
        MatrixStep<T,DEFAULT,N-2, N>(key_tuple, iterator_data_, this)
            ).first->second;
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

  void erase(const KeyTuple& tuple) {
    RecursiveErase(data_, tuple);
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
  using KeyTuple = decltype(make_N_tuple<int, MAX>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>>;
  using MainType = Matrix<T,DEFAULT, MAX>;
 public:
  MatrixStep(KeyTuple tuple, Iterators& iterator_data, MainType* main)
    : key_tuple_(std::move(tuple))
    , iterator_data_(iterator_data)
    , main_(main) {
  }
  ~MatrixStep() = default;

  MatrixStepTDN& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return it->second;
    }
    std::get<MAX-N-1>(key_tuple_) = idx;
    return data_.emplace(idx,
        MatrixStepTDN(key_tuple_, iterator_data_, main_)
        ).first->second;
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

  void erase(int idx) {
    data_.erase(idx);
  }

 private:
  std::map<int, MatrixStepTDN> data_;
  KeyTuple key_tuple_;
  Iterators& iterator_data_;
  MainType* main_;
};

/**
 * @brief Last step element in Matrix specialization
 */
template<typename T, T DEFAULT, int MAX>
class MatrixStep<T,DEFAULT, 0, MAX> {
  using KeyTuple = decltype(make_N_tuple<int, MAX>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>>;
  using MainType = Matrix<T,DEFAULT, MAX>;
 public:
  MatrixStep(KeyTuple tuple, Iterators& iterator_data, MainType* main)
      : key_tuple_(std::move(tuple))
      , iterator_data_(iterator_data)
      , main_(main)  {
  }
  ~MatrixStep() = default;

  MatrixElement<T, DEFAULT, MAX>& operator [](int idx) {
    auto it = data_.find(idx);
    if (it != data_.end()) {
      return it->second;
    }
    std::get<MAX-1>(key_tuple_) = idx;
    return data_.emplace(
        idx, MatrixElement<T, DEFAULT, MAX>(key_tuple_, iterator_data_, main_)
            ).first->second;
  }

  auto find(int idx) {
    return data_.find(idx);
  }

  auto end() {
    return data_.end();
  }

  long size() const {
    return std::count_if(data_.begin(), data_.end(), [](auto i) {return i.second.data_value_ != DEFAULT;});
  }

  void erase(int idx) {
    data_.erase(idx);
  }

 private:
  std::map<int, MatrixElement<T, DEFAULT, MAX>> data_;
  KeyTuple key_tuple_;
  Iterators& iterator_data_;
  MainType* main_;
};
/**@{*/

/**
 * @brief Matrix element data class
 */
template <typename T, T DEFAULT, int MAX>
class MatrixElement {
  using KeyTuple = decltype(make_N_tuple<int, MAX>());
  using Iterators = std::vector<std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>>;
  using MainType = Matrix<T,DEFAULT, MAX>;
 public:
  MatrixElement(KeyTuple tuple, Iterators& iterator_data, MainType* main)
      : data_value_(DEFAULT)
      , key_tuple_(tuple)
      , iterator_data_(iterator_data)
      , main_(main) {
  }
  ~MatrixElement() = default;

  MatrixElement& operator= (const T &value) {
    if (data_value_ == value && value != DEFAULT) {
      return *this;
    }

    data_value_ = value;
    auto it = std::find_if(iterator_data_.begin(), iterator_data_.end(),
        [&] (const std::pair<KeyTuple, MatrixElement<T, DEFAULT, MAX>*>& pair) {
          return (pair.first == key_tuple_);
        });
    if (value == DEFAULT) {
      if (it != iterator_data_.end()) {
        main_->erase(key_tuple_);
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

  operator const T&() const {
    return data_value_;
  };

  long size() {return 0;};

  template <typename U, U DEFAULT_, int MAX_>
  friend std::ostream& operator<<(std::ostream& stream, const MatrixElement<U, DEFAULT_, MAX_>& element);

  T data_value_;
 private:
  KeyTuple key_tuple_;
  Iterators& iterator_data_;
  MainType* main_;
};

template <typename U, U DEFAULT_, int MAX_>
std::ostream& operator<<(std::ostream& stream, const MatrixElement<U, DEFAULT_, MAX_>& element) {
  return stream << element.data_value_;
}

#endif //OTUS_CPP_MATRIX_H
