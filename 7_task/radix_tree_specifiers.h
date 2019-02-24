//
// Created by imelker on 24.02.19.
//

#ifndef OTUS_CPP_RADIX_SPECIFIERS_H
#define OTUS_CPP_RADIX_SPECIFIERS_H

#include <tuple>
#include <string>
#include <string.h>

/**
 * \defgroup specifiers
 * Specifiers for getting radix key length
 */
template<typename K>
int radixKeyLength(const K &key);

template<>
inline int radixKeyLength<std::string>(const std::string &key) {
  return static_cast<int>(key.size());
}
/**@{*/

/**
 * \defgroup specifiers
 * Specifiers for getting key segments on needed depth
 */
template<typename K>
K radixKeySegment(const K &key, int begin, int size);

template<>
inline std::string radixKeySegment<std::string>(const std::string &key, int begin, int size) {
  return key.substr(static_cast<std::size_t>(begin), static_cast<std::size_t>(size));
}
/**@{*/

/**
 * \defgroup specifiers
 * Specifiers for getting equal range of segments
 */
template <typename K>
std::tuple<int, int, int> radixKeySimilarityLength(const K& node_key, const K& new_key, int key_offset);

template<>
std::tuple<int, int, int> radixKeySimilarityLength(const std::string& node_key, const std::string& new_key, int key_offset) {
  int key_len_1 = radixKeyLength(node_key);
  int key_len_2 = radixKeyLength(new_key) - key_offset;
  int same_count;
  for (same_count = 0; same_count < key_len_1 && same_count < key_len_2; same_count++) {
    if (node_key[same_count] != new_key[same_count + key_offset])
      break;
  }
  return std::make_tuple(key_len_1, key_len_2, same_count);
}
/**@{*/

/**
 * \defgroup specifiers
 * Specifiers for checking if segment starts are equal
 */
template <typename K>
bool radixSegmentStartEqual(const K& full_key, int key_offset, const K& segment);

template<>
inline bool radixSegmentStartEqual<std::string>(const std::string& full_key,
                                                int key_offset,
                                                const std::string& segment) {
  return full_key[key_offset] == segment[0];
}
/**@{*/

/**
 * \defgroup specifiers
 * Specifiers for joining segments
 */
template<typename K>
K radixSegmentJoin(const K& lkey, const K& rkey);

template<>
inline std::string radixSegmentJoin<std::string>(const std::string& lkey, const std::string& rkey) {
  return lkey + rkey;
}
/**@{*/
#endif //OTUS_CPP_RADIX_SPECIFIERS_H
