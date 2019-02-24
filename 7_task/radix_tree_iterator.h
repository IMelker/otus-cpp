//
// Created by imelker on 2/8/19.
//

#ifndef OTUS_CPP_RADIX_TREE_ITERATOR_H
#define OTUS_CPP_RADIX_TREE_ITERATOR_H

#include <iterator>
#include <cassert>

template <typename K, typename T> class RadixTree;
template <typename K, typename T> class RadixTreeNode;

template <typename K, typename T>
class RadixTreeIterator : public std::iterator<std::forward_iterator_tag, std::pair<K, T> > {
  friend class RadixTree<K, T>;

  using RadixTreeNodeType = RadixTreeNode<K, T>;
 public:
  RadixTreeIterator() : node_ptr_(nullptr) {};
  ~RadixTreeIterator() = default;

  RadixTreeIterator(const RadixTreeIterator& rhs) : node_ptr_(rhs.node_ptr_) {};
  RadixTreeIterator& operator= (const RadixTreeIterator& rhs);

  std::pair<const K, T>& operator* () const { return *node_ptr_->value_.get(); }
  std::pair<const K, T>* operator-> () const { return node_ptr_->value_.get(); }

  const RadixTreeIterator& operator++ ();
  RadixTreeIterator operator++ (int);

  bool operator!= (const RadixTreeIterator& rhs) const { return node_ptr_ != rhs.node_ptr_; }
  bool operator== (const RadixTreeIterator& rhs) const { return node_ptr_ == rhs.node_ptr_; }
 private:
  explicit RadixTreeIterator(RadixTreeNodeType* node_ptr) : node_ptr_(node_ptr) {};

  RadixTreeNodeType* increment(RadixTreeNodeType* node_ptr) const;
  RadixTreeNodeType* descend(RadixTreeNodeType* node_ptr) const;

  RadixTreeNodeType* node_ptr_;
};

template<typename K, typename T>
RadixTreeIterator<K, T>& RadixTreeIterator<K, T>::operator=(const RadixTreeIterator &rhs) {
  node_ptr_ = rhs.node_ptr_;
  return *this;
}

template<typename K, typename T>
RadixTreeNode<K,T>* RadixTreeIterator<K,T>::increment(RadixTreeNode<K,T>* node_ptr) const {
  RadixTreeNode<K,T>* parent = node_ptr->parent_;

  if (parent == nullptr)
    return nullptr;

  auto it = parent->childs_.find(node_ptr->key_);
  assert(it != parent->childs_.end());

  ++it;

  if (it == parent->childs_.end())
    return increment(parent);
  else
    return descend(it->second.get());
}

template<typename K, typename T>
RadixTreeNode<K,T>* RadixTreeIterator<K,T>::descend(RadixTreeNode<K,T>* node_ptr) const {
  if (node_ptr->is_leaf_)
    return node_ptr;

  auto it = node_ptr->childs_.begin();
  assert(it != node_ptr->childs_.end());

  return descend(it->second.get());
}

template <typename K, typename T>
const RadixTreeIterator<K,T>& RadixTreeIterator<K,T>::operator++() {
  if (node_ptr_ != nullptr)
    node_ptr_ = increment(node_ptr_);
  return *this;
}

template<typename K, typename T>
RadixTreeIterator<K, T> RadixTreeIterator<K, T>::operator++(int)  {
  RadixTreeIterator copy(*this);
  ++(*this);
  return copy;
}

#endif //OTUS_CPP_RADIX_TREE_ITERATOR_H
