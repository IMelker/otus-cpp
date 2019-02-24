//
// Created by imelker on 2/8/19.
//

#ifndef OTUS_CPP_RADIX_TREE_NODE_H
#define OTUS_CPP_RADIX_TREE_NODE_H

#include <memory>
#include <map>

template <typename K, typename T>
class RadixTreeNode {
  friend class RadixTree<K, T>;
  friend class RadixTreeIterator<K, T>;

  using ValueType = std::pair<const K, T>;
 public:
  ~RadixTreeNode() = default;
  RadixTreeNode(const RadixTreeNode&) = delete;
  RadixTreeNode& operator=(const RadixTreeNode&) = delete;

 private:
  RadixTreeNode();
  RadixTreeNode(ValueType value);

  std::map<K, std::unique_ptr<RadixTreeNode<K,T>>, std::less<K>> childs_;
  RadixTreeNode* parent_;
  std::unique_ptr<ValueType> value_;
  K key_;
  int depth_;
  bool is_leaf_;
};

template <typename K, typename T>
RadixTreeNode<K,T>::RadixTreeNode()
    : parent_(nullptr)
    , value_(nullptr)
    , depth_(0)
    , is_leaf_(false) {
}

template<typename K, typename T>
RadixTreeNode<K, T>::RadixTreeNode(ValueType value)
    : childs_(std::map<K, std::unique_ptr<RadixTreeNode<K,T>>, std::less<K>>(std::less<K>()))
    , parent_(nullptr)
    , value_(std::unique_ptr<ValueType>(nullptr))
    , depth_(0)
    , is_leaf_(false) {
  value_ = std::make_unique<ValueType>(std::forward<ValueType>(value));
}

#endif //OTUS_CPP_RADIX_TREE_NODE_H
