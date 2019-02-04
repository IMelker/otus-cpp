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

  SList();
  ~SList();

  SList(SList& other);
  SList& operator=(SList other);

  //SList& operator=(SList&& other) {
  //  SList(std::move(other));
  //  return *this;
  //}

  void Add(Node data);
  template<typename... Args>
  void Emplace(Args&&... args);

  template<typename U, typename B>
  friend std::ostream& operator<<(std::ostream& stream, const SList<U, B>& list);

  class Iterator {
   public:
    Iterator(SList *list)
        : parent(list), ptr(list->head) {
    }
    void operator++() {
      if (ptr != nullptr)
        ptr = ptr->next;
    }
    typename SList::Node& operator*() {
      return *ptr;
    }
    typename SList::Node* operator->() {
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

  RebindAllocNode allocNode;
};

template<typename T, typename A>
SList<T, A>::SList()
    : head(nullptr), tail(nullptr) {
}

template<typename T, typename A>
SList<T, A>::~SList() {
  auto ptr = head;
  while (ptr != nullptr) {
    Node* next = ptr->next;
    Destroy(ptr);
    ptr = next;
  }
}

template<typename T, typename A>
SList<T, A>::SList(SList<T, A>& other) : head(nullptr) {
  auto node = other.head;
  while (node != nullptr) {
    Emplace(node->data);
    node = node->next;
  }
}

template<typename T, typename A>
SList<T, A>& SList<T, A>::operator=(SList<T, A> other) {
  std::swap(head, other.head);
  return *this;
}

template<typename T, typename A>
void SList<T, A>::Destroy(Node* node) {
  allocNode.destroy(node);
  allocNode.deallocate(node, 1);
}

template<typename T, typename A>
void SList<T, A>::Add(Node data) {
  Node *node = allocNode.allocate(1);
  allocNode.construct(node, std::move(data));
  if (tail != nullptr)
    tail->next = node;
  tail = node;
  if (head == nullptr)
    head = node;
}

template<typename T, typename A> template<typename... Args>
void SList<T, A>::Emplace(Args&&... args) {
  Node *node = allocNode.allocate(1);
  allocNode.construct(node, Node{T(std::forward<Args>(args)...), nullptr});
  if (tail != nullptr)
    tail->next = node;
  tail = node;
  if (head == nullptr)
    head = node;
}

template<typename T, typename A>
std::ostream &operator<<(std::ostream& stream, const SList<T, A>& list) {
  for (typename SList<T, A>::Node* ptr = list.head; ptr != nullptr; ptr = ptr->next)
    stream << ptr->data << std::endl;

  return stream;
}

#endif //OTUS_CPP_S_LIST_H
