//
// Created by imelker on 1/11/19.
//

#ifndef OTUS_CPP_S_LIST_H
#define OTUS_CPP_S_LIST_H

template<typename T, typename A = std::allocator<T>>
class SList {
 public:
  struct Node {
    T data;
    Node *next;
  };

  SList(A allocator = A());
  ~SList();

  void Add(const T &data);

  template<typename U, typename B>
  friend std::ostream &operator<<(std::ostream &stream, const SList<U, B> &list);

  class Iterator {
   public:
    Iterator(SList *list)
        : parent(list), ptr(list->head) {
    }
    void operator++() {
      if (ptr != nullptr)
        ptr = ptr->next;
    }
    typename SList::Node &operator*() {
      return *ptr;
    }
    typename SList::Node *operator->() {
      return ptr;
    }
    bool IsEnd() const {
      return ptr == nullptr;
    }
   private:
    SList *parent;
    typename SList::Node *ptr;
  };

  friend class Iterator;

 private:
  void Destroy(Node *node);

  Node *head;
  Node *tail;

  using AllocatorTraits = std::allocator_traits<A>;
  using RebindAllocNode = typename AllocatorTraits::template rebind_alloc<Node>;

  A allocData;
  RebindAllocNode allocNode;
};

template<typename T, typename A>
SList<T, A>::SList(A allocator)
    : head(nullptr), tail(nullptr), allocData(allocator) {
}

template<typename T, typename A>
SList<T, A>::~SList() {
  auto ptr = head;
  while (ptr != nullptr) {
    auto next = ptr->next;
    Destroy(ptr);
    ptr = next;
  }
}

template<typename T, typename A>
void SList<T, A>::Destroy(Node *node) {
  allocData.destroy(node);
  allocNode.deallocate(node, 1);
}

template<typename T, typename A>
void SList<T, A>::Add(const T &data) {
  Node *node = allocNode.allocate(1);
  allocData.construct(node, Node{data, nullptr});
  if (tail != nullptr)
    tail->next = node;
  tail = node;
  if (head == nullptr)
    head = node;
}

template<typename T, typename A>
std::ostream &operator<<(std::ostream &stream, const SList<T, A> &list) {
  for (typename SList<T, A>::Node *ptr = list.head; ptr != nullptr; ptr = ptr->next)
    stream << ptr->data << std::endl;

  return stream;
}

#endif //OTUS_CPP_S_LIST_H
