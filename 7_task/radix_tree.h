//
// Created by imelker on 2/7/19.
//

#ifndef OTUS_CPP_RADIX_TREE_H
#define OTUS_CPP_RADIX_TREE_H

#include <ostream>
#include <memory>
#include <vector>

#include "radix_tree_specifiers.h"
#include "radix_tree_iterator.h"
#include "radix_tree_node.h"

template <typename K, typename T>
class RadixTree {
  using SizeType = std::size_t;
  using ValueType = std::pair<const K, T>;
  using Node = RadixTreeNode<K, T>;
  using Iterator = RadixTreeIterator<K, T>;
 public:
  RadixTree() : size_(0), root_(std::unique_ptr<Node>(nullptr)) {}

  ~RadixTree() = default;
  RadixTree& operator =(const RadixTree other) = delete;
  RadixTree(const RadixTree& other) = delete;

  std::pair<Iterator, bool> insert(ValueType value);
  template<typename... Args>
  std::pair<Iterator, bool> emplace(Args&&... args);

  K getPrefix(Iterator it);

  Iterator find(const K& key);
  Iterator begin();
  Iterator end();

  bool erase(const K& key);
  void erase(Iterator it);
  void clear();

  bool empty() const { return size_ == 0; }
  SizeType size() const { return size_; }

  T& at(const K& lhs) const;
  T& operator[] (const K& lhs);

  static void toString(Node* node, std::string& res);

  template<typename U, typename B>
  friend std::ostream& operator<<(std::ostream& stream, const RadixTree<U,B>& tree);
 private:
  Node* findNode(const K &key, Node* node, int depth);
  template <class P>
  Node* append(Node* parent, P&& value);
  template <class P>
  Node* prepend(Node* node, P&& value);

  SizeType size_;
  std::unique_ptr<Node> root_;
};

template<typename K, typename T>
std::pair<RadixTreeIterator<K, T>, bool> RadixTree<K, T>::insert(ValueType value)  {
  if (!root_) {
    root_ = std::unique_ptr<Node>(new Node());
    root_->key_ = radixKeySegment(value.first, 0, 0);
  }

  auto* node = findNode(value.first, root_.get(), 0);

  if (node->is_leaf_) {
    return std::make_pair(Iterator(node), false);
  } else if (node == root_.get()) {
    size_++;
    return std::make_pair(
        Iterator(append(root_.get(), std::forward<ValueType>(value))),
        true);
  } else {
    size_++;

    int key_len = radixKeyLength(node->key_);
    K key_segment = radixKeySegment(value.first, node->depth_, key_len);

    if (key_segment == node->key_) {
      return std::make_pair(
          Iterator(append(node, std::forward<ValueType>(value))),
          true);
    } else {
      return std::make_pair(
          Iterator(prepend(node, std::forward<ValueType>(value))),
          true);
    }
  }
}

template<typename K, typename T>
template<typename... Args>
std::pair<RadixTreeIterator<K, T>, bool> RadixTree<K, T>::emplace(Args &&... args) {
  return insert(std::make_pair(args...));
}

template<typename K, typename T>
RadixTreeIterator<K, T> RadixTree<K, T>::find(const K& key) {
  if (!root_) {
    return Iterator(nullptr);
  }

  auto node_ptr = findNode(key, root_.get(), 0);

  if(!node_ptr->is_leaf_) {
    return Iterator(nullptr);
  }
  return Iterator(node_ptr);
}

template<typename K, typename T>
RadixTreeIterator<K, T> RadixTree<K, T>::begin() {
  if (root_ && size_ > 0) {
    auto* node_ptr = root_.get();
    while (!node_ptr->is_leaf_) {
      node_ptr = node_ptr->childs_.begin()->second.get();
    }
    return Iterator(node_ptr);
  }
  return Iterator(nullptr);
}

template<typename K, typename T>
RadixTreeIterator<K, T> RadixTree<K, T>::end() {
  return RadixTreeIterator<K, T>(nullptr);
}

template<typename K, typename T>
bool RadixTree<K, T>::erase(const K& key) {
  if (!root_) {
    return false;
  }

  auto child = findNode(key, root_.get(), 0);
  if (!child->is_leaf_) {
    return false;
  }

  K empty_key = radixKeySegment(key, 0, 0);
  auto parent = child->parent_;
  parent->childs_.erase(empty_key);

  size_--;

  if (parent == root_.get() || parent->childs_.size() > 1) {
    // no need to remove upper relations
    return true;
  }

  Node* grandparent;
  if (parent->childs_.empty()) {
    grandparent = parent->parent_;
    grandparent->childs_.erase(parent->key_);
  } else {
    grandparent = parent;
  }

  if (grandparent == root_.get()) {
    return true;
  }

  if (grandparent->childs_.size() == 1) {
    // merge grandparent with the uncle
    auto it = grandparent->childs_.begin();

    Node* uncle = it->second.get();
    if (uncle->is_leaf_) {
      return true;
    }
    uncle->depth_ = grandparent->depth_;
    uncle->key_ = radixSegmentJoin(grandparent->key_, uncle->key_);
    uncle->parent_ = grandparent->parent_;

    grandparent->parent_->childs_[uncle->key_] = std::move(it->second);
    grandparent->parent_->childs_.erase(grandparent->key_);
  }
  return true;
}

template<typename K, typename T>
void RadixTree<K, T>::erase(Iterator it) {
  erase(it->first);
}

template<typename K, typename T>
void RadixTree<K, T>::clear() {
  root_.reset(nullptr);
  size_ = 0;
}

template<typename K, typename T>
T& RadixTree<K, T>::at(const K& lhs) const {
  auto it = find(lhs);
  assert(it != end());
  return it->second;
}

template<typename K, typename T>
T& RadixTree<K, T>::operator[] (const K& lhs) {
  auto it = find(lhs);
  if (it == end()) {
    auto ret = emplace(lhs, T());
    it = ret.first;
  }
  return it->second;
}

template<typename K, typename T>
RadixTreeNode<K, T>* RadixTree<K, T>::findNode(const K& key, Node* node, int depth) {
  if (node->childs_.empty()) {
    return node;
  }

  int len_seg = radixKeyLength(key) - depth;

  for (const auto& pair : node->childs_) {
    if (len_seg == 0) {
      if (pair.second->is_leaf_)
        return pair.second.get();
      else
        continue;
    }

    if (!pair.second->is_leaf_ && radixSegmentStartEqual(key, depth, pair.first) ) {
      int len_node = radixKeyLength(pair.first);
      K key_seg  = radixKeySegment(key, depth, len_node);

      if (key_seg == pair.first) {
        return findNode(key, pair.second.get(), depth + len_node);
      } else {
        return pair.second.get();
      }
    }
  }

  return node;
}

template<typename K, typename T>
template <class P>
RadixTreeNode<K, T>* RadixTree<K, T>::append(Node* parent, P&& value) {
  K empty_key = radixKeySegment(value.first, 0, 0);

  int depth = parent->depth_ + radixKeyLength(parent->key_);
  int len_seg = radixKeyLength(value.first) - depth;

  if (len_seg != 0) {
    K key_segment = radixKeySegment(value.first, depth, len_seg);
    auto* additional_node = new Node();
    additional_node->depth_  = depth;
    additional_node->key_ = key_segment;
    additional_node->parent_ = parent;
    parent->childs_[key_segment] = std::unique_ptr<Node>(additional_node);

    parent = additional_node;
  }

  auto* appended_node = new Node(std::forward<P>(value));
  appended_node->depth_ = depth + len_seg;
  appended_node->key_ = empty_key;
  appended_node->is_leaf_ = true;
  appended_node->parent_ = parent;
  parent->childs_[empty_key] = std::unique_ptr<Node>(appended_node);
  return appended_node;
}

template<typename K, typename T>
template <class P>
RadixTreeNode<K, T>* RadixTree<K, T>::prepend(Node* node, P&& value) {
  int seg_len_1, seg_len_2, same_count;
  std::tie(seg_len_1, seg_len_2, same_count) = radixKeySimilarityLength(node->key_, value.first, node->depth_);
  assert(same_count != 0);

  // add new node
  auto *displacing_node = new Node();
  displacing_node->key_ = radixKeySegment(node->key_, 0, same_count);
  displacing_node->depth_ = node->depth_;
  displacing_node->parent_ = node->parent_;
  displacing_node->parent_->childs_[displacing_node->key_] = std::unique_ptr<Node>(displacing_node);

  // move connection from node->parent to displacing_node
  auto new_key = radixKeySegment(node->key_, same_count, seg_len_1 - same_count);
  displacing_node->childs_[new_key] = std::move(node->parent_->childs_.at(node->key_));
  node->parent_->childs_.erase(node->key_);

  // change node data for key, depth and new parent
  node->depth_ += same_count;
  node->key_ = std::move(new_key);
  node->parent_ = displacing_node;

  // add new node with data
  auto* parent = displacing_node;
  K empty_key = radixKeySegment(value.first, 0, 0);
  if (same_count != seg_len_2) {
    auto* additional_node = new Node();
    additional_node->parent_ = parent;
    additional_node->depth_ = node->depth_;
    additional_node->key_ = radixKeySegment(value.first, additional_node->depth_, seg_len_2 - same_count);
    additional_node->parent_->childs_[additional_node->key_] = std::unique_ptr<Node>(additional_node);

    parent = additional_node;
  }

  auto* appended_node = new Node(std::forward<P>(value));
  appended_node->depth_ = displacing_node->depth_ + seg_len_2;
  appended_node->parent_  = parent;
  appended_node->key_     = empty_key;
  appended_node->is_leaf_ = true;
  appended_node->parent_->childs_[empty_key] = std::unique_ptr<Node>(appended_node);

  return appended_node;
}

template<typename K, typename T>
K RadixTree<K, T>::getPrefix(Iterator it) {
  auto full_key = it->first;
  if(it.node_ptr_ == nullptr) {
    return full_key;
  }

  auto parent = it.node_ptr_->parent_;
  if (parent == root_.get() || parent == nullptr) {
    return radixKeySegment(full_key, 0, 1);
  }
  if(parent->childs_.size() > 1) {
    return full_key;
  }
  if (!parent->is_leaf_) {
    return radixKeySegment(full_key, 0, parent->depth_ + 1);
  } else {
    return radixKeySegment(full_key, 0, 1);
  }
}

template<typename K, typename T>
void RadixTree<K, T>::toString(RadixTree::Node *parent, std::string &res) {
  static std::string depth;
  std::string depth_temp = depth;

  res += parent->key_;
  if (parent->childs_.size() > 0 && parent->childs_.begin()->second->is_leaf_) {
    res += '$';
  }
  res += '\n';

  std::size_t count = 0;
  for (const auto& node: parent->childs_) {
    auto node_ptr = node.second.get();
    if(!node_ptr->is_leaf_) {
      if (count < parent->childs_.size() - 1) {
        res += depth + "├─";
        depth += "│ ";
      } else {
        res += depth + "└─";
        depth += "  ";
      }
      toString(node_ptr, res);
      depth = depth_temp;
    }
    ++count;
  }
}

template<typename T, typename U>
std::ostream &operator<<(std::ostream& stream, const RadixTree<T, U>& tree) {
  if (!tree.root_) {
    return stream;
  }

  std::string tree_str;
  tree.toString(tree.root_.get(), tree_str);
  stream << tree_str;
  return stream;
}

#endif //OTUS_CPP_RADIX_TREE_H
