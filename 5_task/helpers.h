//
// Created by imelker on 2/5/19.
//

#ifndef OTUS_CPP_HELPERS_H
#define OTUS_CPP_HELPERS_H

#include <tuple>

#ifdef DEBUG_LOG
template <typename Arg, typename... Args>
void log(Arg&& arg, Args&&... args) {
  std::cout << std::forward<Arg>(arg);
  using expander = int[];
  (void)expander{0, (void(std::cout << std::forward<Args>(args)), 0)...};
  std::cout << std::endl;
}
#define LOG(...) log(__VA_ARGS__)
#else
#define LOG(...)
#endif /* DEBUG_LOG */

/**
* @brief  Helper for creating a tuple of N Elements
*/
template<size_t, class T>
using T_ = T;

template<class T, size_t... Is>
auto make_N_tuple(std::index_sequence<Is...>) { return std::tuple<T_<Is, T>...>{}; }

template<class T, size_t N>
auto make_N_tuple() { return make_N_tuple<T>(std::make_index_sequence<N>{}); }
/**@{*/

/**
* @brief  Helper function to print a tuple of any size
*/
template<class Tuple, std::size_t N>
struct TuplePrinter {
  static void print(const Tuple& t)
  {
    TuplePrinter<Tuple, N-1>::print(t);
    std::cout << ", " << std::get<N-1>(t);
  }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
  static void print(const Tuple& t)
  {
    std::cout << std::get<0>(t);
  }
};

template<class... Args>
void print(const std::tuple<Args...>& t)
{
  std::cout << "(";
  TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
  std::cout << ")\n";
}
/**@{*/


#endif //OTUS_CPP_HELPERS_H
