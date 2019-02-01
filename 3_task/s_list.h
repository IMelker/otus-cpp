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

  SList(const SList& other) {
    DestroyLinkedNodes(head);
    head = nullptr;

    auto node = other.head;
    while (node != nullptr) {
      Add(node->data);
      node = node->next;
    }
  }

  SList& operator=(SList other) {
    std::swap(head, other.Head);
    return *this;
  }

  SList(SList&& other) {
    auto ptr = head;
    auto other_ptr = other.head;
    while (ptr != nullptr) {
      auto next = ptr->next;

      if (other_ptr == nullptr) {
        DestroyLinkedNodes(ptr->next);
        ptr->next = nullptr;
        break;
      }
      auto other_next = other_ptr->next;

      ptr = other_ptr;
      ptr->next = next;
      other_ptr = other_next;

      ptr = ptr->next;
    }
    while (other_ptr != nullptr) {
      auto other_next = other_ptr->next;

      if (ptr == nullptr) {
        Add(std::move(other_ptr->data));
        other_ptr->data = T();
      } else {
        auto next = ptr->next;
        ptr = other_ptr;
        ptr->next = next;
      }
      other_ptr = other_next;
      ptr = ptr->next;
    }
  }

  SList& operator=(SList&& other) {
    SList(std::move(other));
    return *this;
  }

  template<typename U = T> //for type inference
  void Add(U&& data);
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
  void DestroyLinkedNodes(Node *node);

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
  DestroyLinkedNodes(head);
}

template<typename T, typename A>
void SList<T, A>::Destroy(Node* node) {
  allocNode.destroy(node);
  allocNode.deallocate(node, 1);
}

template<typename T, typename A>
void SList<T, A>::DestroyLinkedNodes(Node *node) {
  auto ptr = node;
  while (ptr != nullptr) {
    auto next = ptr->next;
    Destroy(ptr);
    ptr = next;
  }
}

template<typename T, typename A> template<typename U>
void SList<T, A>::Add(U&& data) {
  Node *node = allocNode.allocate(1);
  allocNode.construct(node, Node{std::forward<U>(data), nullptr});
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
