#ifndef ALLOCATOR_MY_STD_ALLOCATOR_H
#define ALLOCATOR_MY_STD_ALLOCATOR_H

#include <iostream>
#include <cstring>

template<typename T>
class MyStdAllocator {
 public:
  using value_type = T;
  using size_type = std::size_t;
  //using difference_type = ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using Traits = std::allocator_traits<MyStdAllocator<T>>;
  template<typename U>
  struct rebing{
    using other = MyStdAllocator<U>;
  };

  MyStdAllocator(){}

  explicit MyStdAllocator(T& allocator)/* : m_allocator(&allocator) */{}
  template<typename U>
  MyStdAllocator(const MyStdAllocator<U>& other)/* : m_allocator(other.m_allocator) */{}

  pointer allocate(size_type n) {
    if( n > (allocations + unused_allocations)) {
      std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "\n";
      auto p  = std::malloc(n * sizeof(T));
      if (!p) throw std::bad_alloc();

      if(unused_allocations != 0 || allocations !=0 && current_p) {
        std::memcpy(p, current_p, allocations * sizeof(T));
        deallocate(reinterpret_cast<pointer>(current_p), allocations);
      }
      unused_allocations = n;
      allocations = 0;
      current_p = p;
      return reinterpret_cast<pointer>(p);
    } else {
      return reinterpret_cast<pointer>(current_p + allocations);
    }
  }

  void deallocate(pointer p, size_type n) {
    std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "\n";
    std::free(p);
  }
  template <typename U, typename ... Args>
  void construct(U* p, Args&& ... args) {
    std::cout << __PRETTY_FUNCTION__ << "\n";
    new(p + allocations) U(std::forward<Args>(args) ... );
    allocations++;
    unused_allocations--;
  }
  void destroy(pointer p) {
    std::cout << __PRETTY_FUNCTION__ << "\n";
    p->~T();
    allocations--;
    unused_allocations++;
  }

  void reserve(size_t n) {
    allocate(n);
  }

 private:
  void* current_p = nullptr;
  int allocations = 0;
  int unused_allocations = 0;
};

#endif //ALLOCATOR_MY_STD_ALLOCATOR_H
