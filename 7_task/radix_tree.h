//
// Created by imelker on 2/7/19.
//

#ifndef OTUS_CPP_RADIX_TREE_H
#define OTUS_CPP_RADIX_TREE_H

#include <ostream>
#include <memory>
#include <vector>

#include "radix_tree_node.h"
#include "radix_tree_iterator.h"

template <typename K, typename T, typename Compare = std::less<K>>
class RadixTree {
  typedef std::size_t size_type;
  typedef RadixTreeIterator<K, T, Compare> iterator;
  typedef RadixTreeNode<K, T, Compare> radix_tree_node;
 public:
  RadixTree() : root_(std::unique_ptr<radix_tree_node>(nullptr)) {}
  ~RadixTree() = default;

  // remove copy/move constuctors/operators
  RadixTree& operator =(const RadixTree other) = delete;
  RadixTree(const RadixTree& other) = delete;

  std::pair<iterator, bool> insert(std::pair<K, T> value) {
    if (!root_) {
      root_ = std::make_unique<radix_tree_node>();
    }

    // есть ключ
    // делаем поиск по дереву
    // сравниваем у детей по принципу: node_key == value.first.sub(0,node_key.length())
    // если ноды далее последней нет, то подвешиваем новую отрезав начало
    // если ключ является только частью, то разрезаем существующую ноду и генерим новую
    // если есть, то делаем присвоение ноде значение

    // возвращаем указатель с флагом перезаписи



    return std::pair<iterator, bool>();
  }


  template<typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    return insert(std::make_pair<K, T>(args...));
  }

  bool erase(const K& key) {

    return false;
  }
  void erase(iterator it) {

  }

  K get_prefix(const K &key) {

    return nullptr;
  }

  std::vector<iterator> prefix_match(const K &key) {

    return std::vector<iterator>();
  }

  std::vector<iterator> greedy_match(const K &key) {

    return std::vector<iterator>();
  }

  iterator find(const K& key) {
    if (!root_) {
      return iterator(nullptr);
    }

    auto node_ptr = find_node(key, root_.get(), 0);

    if(!node_ptr->is_leaf_) {
      return iterator(nullptr);
    }
    return iterator(node_ptr);
  }

  iterator begin() {
    if (root_ && size_ > 0) {
      auto node_ptr = root_.get();
      while (!node_ptr->is_leaf_) {
        node_ptr = node_ptr->childs_.begin().second.get();
      }
      return iterator(node_ptr);
    }
    return iterator(nullptr);
  }

  iterator end() {
    return iterator(nullptr);
  }

  void clear() {
    root_.reset(nullptr);
    size_ = 0;
  }

  bool empty() const {
    return size_ == 0;
  }

  size_type size() const {
    return size_;
  }

  T& at(const K& lhs) const {
    auto it = find(lhs);
    assert(it != end());
    return it->second;
  }

  T& operator[] (const K& lhs) {
    auto it = find(lhs);
    if (it == end()) {
      auto ret = emplace(lhs, T());
      it = ret.first;
    }
    return it->second;
  }

  template<typename U, typename B, typename C>
  friend std::ostream& operator<<(std::ostream& stream, const RadixTree<U,B,C>& tree);
 private:
  size_type size_;
  std::unique_ptr<radix_tree_node> root_;

  radix_tree_node* find_node(const K &key, radix_tree_node* node, int depth) {

  }
};

template<typename T, typename U, typename Compare>
std::ostream &operator<<(std::ostream& stream, const RadixTree<T, U, Compare>& tree) {

  return stream;
}

#endif //OTUS_CPP_RADIX_TREE_H
