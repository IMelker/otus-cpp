//
// Created by imelker on 2/8/19.
//

#ifndef OTUS_CPP_RADIX_TREE_NODE_H
#define OTUS_CPP_RADIX_TREE_NODE_H

#include <memory>
#include <map>

template <typename K, typename T, typename Compare>
class RadixTreeNode {
 public:
  RadixTreeNode();
  ~RadixTreeNode() = default;

  RadixTreeNode(const RadixTreeNode&) = delete;
  RadixTreeNode& operator=(const RadixTreeNode&) = delete;

 private:
  std::unique_ptr<T> data_;
  int depth_;
  bool is_leaf_;
  std::map<K, std::unique_ptr<RadixTreeNode<K,T,Compare>>, Compare> childs_;
};

template <typename K, typename T, typename Compare>
RadixTreeNode<K,T,Compare>::RadixTreeNode()
    : data_(std::make_unique<T>(nullptr))
    , depth_(0)
    , is_leaf_(false) {

}

#endif //OTUS_CPP_RADIX_TREE_NODE_H
