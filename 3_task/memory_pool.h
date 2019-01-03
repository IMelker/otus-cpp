//
// Created by Igor on 10.11.2018.
//
#pragma once

#include <array>
#include <map>
#include <iostream>

template <typename T, size_t N>
class MemoryPool
{
 public:
  static MemoryPool& GetInstance()
  {
    if (instance)
      return *instance;

    instance = new MemoryPool();
    return *instance;
  }

  using Chunk = std::array<T, N>;

  T* Alloc(size_t n)
  {
    std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << "\n";
    for (auto& chunk : memoryPool)
    {
      auto& ar = *chunk.first;
      if (chunk.second + n <= N)
      {
        T* ptr = &ar[0] + chunk.second;
        std::cout << "--- allocated ready: " << ptr << ", "
                  << "chunk.first: " << chunk.first << ", "
                  << "chunk.second: " << chunk.second << std::endl;
        chunk.second += n;
        return ptr;
      }
    }
    return AppendNewChunk(n);
  }

  void Dealloc(T* p)
  {
    std::cout << __PRETTY_FUNCTION__ << "\n--- deallocate: " << p;
    auto chunk = std::prev(memoryPool.upper_bound(reinterpret_cast<Chunk*>(p)));
    std::cout << " deallocate, chunk: " << chunk->first << std::endl;
    if (--chunk->second == 0)
    {
      delete chunk->first;
      memoryPool.erase(chunk);
    }
  }

 private:
  MemoryPool() {}
  ~MemoryPool()
  {
    std::cout << __PRETTY_FUNCTION__;
    for (auto& chunk : memoryPool)
    {
      delete chunk->first;
    }
  }

  MemoryPool(const MemoryPool&) = delete;
  MemoryPool(MemoryPool&&) = delete;
  MemoryPool& operator = (const MemoryPool&) = delete;
  MemoryPool& operator = (MemoryPool&&) = delete;

  T* AppendNewChunk(size_t n)
  {
    std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]\n";
    auto newChunk = new Chunk();
    memoryPool.emplace(std::make_pair(newChunk, n));
    auto& ar = *newChunk;
    auto ptr = &ar[0];
    std::cout << "--- allocated new: " << ptr << std::endl;
    return ptr;
  }

 private:
  std::map<Chunk*, size_t> memoryPool;  // Chunk -> countIssued, for quick deallocation

  static MemoryPool* instance;
};

template <typename T, size_t N>
MemoryPool<T, N>* MemoryPool<T, N>::instance = nullptr;