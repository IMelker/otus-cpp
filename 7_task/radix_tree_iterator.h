//
// Created by imelker on 2/8/19.
//

#ifndef OTUS_CPP_RADIX_TREE_ITERATOR_H
#define OTUS_CPP_RADIX_TREE_ITERATOR_H

#include <iterator>

template <typename K, typename T, class Compare = std::less<K> >
class RadixTreeIterator : public std::iterator<std::forward_iterator_tag, std::pair<K, T> > {
 public:

 private:
};

#endif //OTUS_CPP_RADIX_TREE_ITERATOR_H
