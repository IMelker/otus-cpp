//
// Created by Igor on 10.11.2018.
//
#pragma once

#include <array>
#include <map>
#include <iostream>

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

template<typename T, size_t N>
class MemoryPool {
 public:
  static MemoryPool &GetInstance() {
    if (instance)
      return *instance;

    instance = new MemoryPool();
    return *instance;
  }

  using Chunk = std::array<T, N>;

  T *Alloc(size_t n) {
    LOG(__PRETTY_FUNCTION__, "[n = ", n , "]");
    for (auto &chunk : memoryPool) {
      auto &ar = *chunk.first;
      if (chunk.second + n <= N) {
        T *ptr = &ar[0] + chunk.second;
        LOG("--- already allocated: ", ptr, "chunk_start: ", chunk.first, ", ", "chunk_elem: ", chunk.second);
        chunk.second += n;
        return ptr;
      }
    }
    return AppendNewChunk(n);
  }

  void Dealloc(T *p) {
    auto chunk = std::prev(memoryPool.upper_bound(reinterpret_cast<Chunk *>(p)));
    LOG(__PRETTY_FUNCTION__, "\n--- deallocate: ", p, " deallocate, chunk: ", chunk->first);
    if (--chunk->second == 0) {
      delete chunk->first;
      memoryPool.erase(chunk);
    }
  }

 private:
  MemoryPool() = default;
  ~MemoryPool() {
    LOG(__PRETTY_FUNCTION__);
    for (auto &chunk : memoryPool) {
      delete chunk->first;
    }
  }

  MemoryPool(const MemoryPool &) = delete;
  MemoryPool(MemoryPool &&) = delete;
  MemoryPool &operator=(const MemoryPool &) = delete;
  MemoryPool &operator=(MemoryPool &&) = delete;

  T *AppendNewChunk(size_t n) {
    LOG(__PRETTY_FUNCTION__, "[n = ", n, "]");
    auto newChunk = new Chunk();
    memoryPool.emplace(newChunk, n);
    auto &ar = *newChunk;
    auto ptr = &ar[0];
    LOG("--- new allocate: ", ptr);
    return ptr;
  }

 private:
  std::map<Chunk*, size_t> memoryPool;

  static MemoryPool *instance;
};

template<typename T, size_t N>
MemoryPool<T, N> *MemoryPool<T, N>::instance = nullptr;