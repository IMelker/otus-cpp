//
// Created by Igor on 10.11.2018.
//

#ifndef ALLOCATOR_CHUNKED_ALLOCATOR_H
#define ALLOCATOR_CHUNKED_ALLOCATOR_H

#include "memory_pool.h"

// Allocates memory by N-byte chunks
template <typename T, size_t N>
class ChunkedAllocator {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using Traits = std::allocator_traits<ChunkedAllocator<T, N>>;
  template<typename U>
  struct rebind
  {
    typedef ChunkedAllocator<U, N> other;
  };

  ChunkedAllocator() = default;
  ~ChunkedAllocator() {}

  ChunkedAllocator(const ChunkedAllocator&) = default;
  template <typename U, size_t S>
  ChunkedAllocator(const ChunkedAllocator<U, S>&) {}
  ChunkedAllocator& operator = (const ChunkedAllocator& other) = default;

  pointer allocate(size_type n, const void* /*hint*/ = 0);
  void deallocate(pointer p, size_type /*n*/);

  template<typename U, typename... Args>
  void construct(U* p, Args&&... args);

  template<typename U>
  void destroy(U* p);
};

template <typename T, size_t N>
typename ChunkedAllocator<T, N>::pointer ChunkedAllocator<T, N>
    ::allocate(size_type n, const void* /*hint*/) {
  std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
  return MemoryPool<T, N>::GetInstance().Alloc(n);
}

template <typename T, size_t N>
void ChunkedAllocator<T, N>
    ::deallocate(pointer p, size_type) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  MemoryPool<T, N>::GetInstance().Dealloc(p);
}

template <typename T, size_t N>
template <typename U, typename... Args>
void ChunkedAllocator<T, N>
    ::construct(U* p, Args&&... args) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  ::new((void*)p) U(std::forward<Args>(args)...);
}

template <typename T, size_t N>
template <typename U>
void ChunkedAllocator<T, N>
    ::destroy(U* p) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  p->~U();
}

// std::allocator<void>
template<size_t N>
class ChunkedAllocator<void, N>
{
 public:
  typedef void* pointer;
  typedef const void* const_pointer;
  typedef void value_type;
  template<typename U>
  struct rebind
  {
    typedef ChunkedAllocator<U, N> other;
  };
};

template<typename T, size_t N, typename U, size_t S>
inline bool operator == (const ChunkedAllocator<T, N>&, const ChunkedAllocator<U, S>&)
{
  return true;
}

template<typename T, size_t N, typename U, size_t S>
inline bool operator != (const ChunkedAllocator<T, N>&, const ChunkedAllocator<U, S>&)
{
  return false;
}

#endif //ALLOCATOR_CHUNKED_ALLOCATOR_H
