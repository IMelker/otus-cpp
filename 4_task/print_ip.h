/**
 * @file print_ip.h
 * @author Igor Melker
 * @date 14 Jan 2019
 * @brief Methods for printing ip
 *
 * Methods for printing ip addresses stored in different ways.
 */

#ifndef OTUS_CPP_PRINT_IP_H
#define OTUS_CPP_PRINT_IP_H

#include <string>
#include <iostream>
#include <type_traits>

static const char separator = '.';

/**
 * \defgroup helpers
 * Helper metafunction to cometic changes excluding cpp17
 */
template< class T >
constexpr bool is_integral_v = std::is_integral<T>::value;

template< class T, class U >
constexpr bool is_same_v = std::is_same<T, U>::value;
/**@{*/

/**
 * \defgroup helpers
 * Helper metafunction to check if all types of tuple is same
 */
template<class... Types>
struct has_same_types;

template<class T, class... Types>
struct has_same_types<T, T, Types...> : has_same_types<T, Types...> {};

template<class T, class U, class... Types>
struct has_same_types<T, U, Types...> : std::false_type {};

template<class T>
struct has_same_types<T, T> : std::true_type {};

template<class T, class... Types>
constexpr bool has_same_types_v = has_same_types<T, Types...>::value;
/**@{*/

/**
 * \defgroup helpers
 * Helper function to print a tuple of any size
 */
template<class Tuple, std::size_t N>
struct PrintTuple {
  static void print(const Tuple& t) {
    PrintTuple<Tuple, N-1>::print(t);
    std::cout << separator << std::get<N-1>(t);
  }
};

template<class Tuple>
struct PrintTuple<Tuple, 1> {
  static void print(const Tuple& t) {
    std::cout << std::get<0>(t);
  }
};
/**@}*/

/**
 * @brief  Prints ip address stored as a tuple.
 * @param tuple Ip address represented as tuple
 */
template <typename... Types>
std::enable_if_t<has_same_types_v<Types...>, void> PrintIp(const std::tuple<Types...>& tuple) {
  PrintTuple<decltype(tuple), sizeof...(Types)>::print(tuple);
  std::cout << std::endl;
}

/**
 * @brief Prints ip address stored as std container like list or vector
 * @param container Ip address represented as stl container
 */
template<template<typename, typename> class ContainerT, typename ValueT>
void PrintIp(const ContainerT<ValueT, std::allocator<ValueT>>& container) {
  for (auto it = container.cbegin(); it != container.cend(); ++it) {
    if (it != container.cbegin()) {
      std::cout << separator;
    }
    std::cout << *it;
  }
  std::cout << std::endl;
}

/**
 *  @brief Prints ip address stored as integral type
 *  @param ip Ip address represented as integral type
 */
template <typename T>
std::enable_if_t<is_integral_v<T>, void> PrintIp(const T& ip) {
  static const size_t constexpr octets_count = sizeof(T);
  for (size_t i = octets_count; i--;) {
    std::cout << (ip >> (i << 3) & 0xFF);
    if (i > 0) {
      std::cout << separator;
    }
  }
  std::cout << std::endl;
}

/**
 *  @brief Prints ip address stored as std::string
 *  @param ip Ip address represented as std::string
 */
template <typename T>
std::enable_if_t<is_same_v<T, std::string>, void>  PrintIp(const T& ip) {
  std::cout << ip << std::endl;
}
#endif //OTUS_CPP_PRINT_IP_H
