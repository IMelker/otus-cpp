//
// Created by imelker on 2/7/19.
//

#ifndef OTUS_CPP_RADIX_TREE_H
#define OTUS_CPP_RADIX_TREE_H

#include <ostream>

template <typename K, typename T, typename Compare>
class RadixTree {
  typedef std::size_t size_type;
  typedef int iterator;
  typedef RadixTreeNode<K, T, Compare> radix_tree_node;
 public:
  RadixTree();
  ~RadixTree() = default;

  // remove copy/move constuctors/operators
  RadixTree& operator =(const RadixTree other) = delete;
  RadixTree(const RadixTree& other) = delete;

  std::pair<iterator, bool> insert(std::pair<K, T> pair);
  template<typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args);

  bool erase(const K& key);
  void erase(iterator it);

  K get_prefix(const K &key);
  std::vector<iterator> prefix_match(const K &key);
  std::vector<iterator> greedy_match(const K &key;
  iterator find(const K& key);
  iterator begin();
  iterator end();

  void clear();
  bool empty();
  size_type size();

  T& operator[] (const K& lhs);
  const T& operator[] (const K& lhs);

  template<typename U, typename B>
  friend std::ostream& operator<<(std::ostream& stream, const RadixTree<U, B>& tree);
 private:
  size_type m_size;
  std::unique_ptr<radix_tree_node> m_root;
};

#endif //OTUS_CPP_RADIX_TREE_H
