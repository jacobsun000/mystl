module;

#include <iterator>
#include <limits>

import type;
import utility;
import concepts;

export module list;

export namespace mystl {

// TODO: Allocator
template <class T> struct List {
  using ValueType = T;
  using SizeType = usize;

private:
  struct ListNode {
    ListNode *m_next;
    ListNode *m_prev;
    T m_value;

    inline T &value() { return m_value; }
    inline T const &value() const { return m_value; }
  };

  ListNode m_dummy;
  usize m_size;

  auto *newNode() { return new ListNode; }

  void deleteNode(ListNode *node) noexcept { delete node; }

public:
  List() noexcept {
    m_size = 0;
    m_dummy.m_prev = m_dummy.m_next = &m_dummy;
  }

  List(List &&that) noexcept { _uninit_move_assign(move(that)); }

  template <class L>
    requires container<L> && (std::is_same_v<typename L::ValueType, T>)
  List(L list) {
    _uninit_assign(list.begin(), list.end());
  }

  explicit List(usize n) { _uninit_assign(n); }

  explicit List(usize n, T const &val) { _uninit_assign(n, val); }

  List(List const &that) { _uninit_assign(that.begin(), that.end()); }

  List &operator=(List &&that) {
    clear();
    _uninit_move_assign(move(that));
  }

  List &operator=(List const &that) { assign(that.begin(), that.end()); }

  bool empty() const noexcept { return m_dummy.m_next == &m_dummy; }

  T &front() noexcept { return m_dummy.m_next->value(); }

  T &back() noexcept { return m_dummy.m_prev->value(); }

  T const &front() const noexcept { return m_dummy.m_next->value(); }

  T const &back() const noexcept { return m_dummy.m_prev->value(); }

  usize size() const noexcept { return m_size; }

  constexpr usize max_size() const noexcept {
    return std::numeric_limits<usize>::max();
  }

  template <class... Args> T &push_back(Args &&...args) {
    ListNode *node = newNode();
    ListNode *prev = m_dummy.m_prev;
    prev->m_next = node;
    node->m_prev = prev;
    node->m_next = &m_dummy;
    std::construct_at(&node->value(), forward<Args>(args)...);
    m_dummy.m_prev = node;
    ++m_size;
    return node->value();
  }

  template <class... Args> T &push_front(Args &&...args) {
    ListNode *node = newNode();
    ListNode *next = m_dummy.m_next;
    next->m_prev = node;
    node->m_next = next;
    node->m_prev = &m_dummy;
    std::construct_at(&node->value(), forward<Args>(args)...);
    m_dummy.m_next = node;
    ++m_size;
    return node->value();
  }

  ~List() noexcept { clear(); }

  void clear() noexcept {
    ListNode *curr = m_dummy.m_next;
    while (curr != &m_dummy) {
      std::destroy_at(&curr->value());
      auto next = curr->m_next;
      deleteNode(curr);
      curr = next;
    }
    m_dummy.m_prev = m_dummy.m_next = &m_dummy;
    m_size = 0;
  }

  struct Iterator {
    using ValueType = T;

  private:
    ListNode *m_curr;

    friend List;

    explicit Iterator(ListNode *curr) noexcept : m_curr(curr) {}

  public:
    Iterator() = default;

    Iterator &operator++() noexcept {
      m_curr = m_curr->m_next;
      return *this;
    }

    Iterator operator++(int) noexcept {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    Iterator &operator--() noexcept {
      m_curr = m_curr->m_prev;
      return *this;
    }

    Iterator operator--(int) noexcept {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    T &operator*() const noexcept { return m_curr->value(); }

    bool operator!=(Iterator const &that) const noexcept {
      return m_curr != that.m_curr;
    }

    bool operator==(Iterator const &that) const noexcept {
      return !(*this != that);
    }
  };

  Iterator begin() noexcept { return Iterator{m_dummy.m_next}; }

  Iterator end() noexcept { return Iterator{&m_dummy}; }

  Iterator erase(Iterator pos) noexcept {
    ListNode *node = pos.m_curr;
    auto next = node->m_next;
    auto prev = node->m_prev;
    prev->m_next = next;
    next->m_prev = prev;
    std::destroy_at(&node->value());
    deleteNode(node);
    --m_size;
    return Iterator{next};
  }

  Iterator erase(Iterator first, Iterator last) noexcept {
    while (first != last) {
      first = erase(first);
    }
    return Iterator(first);
  }

  void pop_front() noexcept { erase(begin()); }

  void pop_back() noexcept { erase(std::prev(end())); }

  usize remove(T const &val) noexcept {
    auto first = begin();
    auto last = end();
    usize count = 0;
    while (first != last) {
      if (*first == val) {
        first = erase(first);
        ++count;
      } else {
        ++first;
      }
    }
    return count;
  }

  template <class Fn>
    requires predicate<Fn, T const &>
  usize remove_if(Fn &&fn) noexcept {
    auto first = begin();
    auto last = end();
    usize count = 0;
    while (first != last) {
      if (pred(*first)) {
        first = erase(first);
        ++count;
      } else {
        ++first;
      }
    }
    return count;
  }

  template <class... Args> Iterator emplace(Iterator pos, Args &&...args) {
    ListNode *curr = newNode();
    ListNode *next = pos.m_curr;
    ListNode *prev = next->m_prev;
    curr->m_prev = prev;
    prev->m_next = curr;
    curr->m_next = next;
    next->m_prev = curr;
    std::construct_at(&curr->value(), forward<Args>(args)...);
    ++m_size;
    return Iterator{curr};
  }

  Iterator insert(Iterator pos, const T &val) { return emplace(pos, val); }

  Iterator insert(Iterator pos, T &&val) { return emplace(pos, move(val)); }

  Iterator insert(Iterator pos, usize n, T const &val) {
    auto orig = pos;
    bool had_orig = false;
    while (n) {
      pos = emplace(pos, val);
      if (!had_orig) {
        had_orig = true;
        orig = pos;
      }
      ++pos;
      --n;
    }
    return Iterator(orig);
  }

private:
  void _uninit_move_assign(List &&that) {
    auto prev = that.m_dummy.m_prev;
    auto next = that.m_dummy.m_next;
    prev->m_next = &m_dummy;
    next->m_prev = &m_dummy;
    m_dummy = that.m_dummy;
    that.m_dummy.m_prev = that.m_dummy.m_next = &that.m_dummy;
    m_size = that.m_size;
    that.m_size = 0;
  }

  template <std::input_iterator InputIt>
  void _uninit_assign(InputIt first, InputIt last) {
    m_size = 0;
    ListNode *prev = &m_dummy;
    while (first != last) {
      ListNode *node = newNode();
      prev->m_next = node;
      node->m_prev = prev;
      std::construct_at(&node->value(), *first);
      prev = node;
      ++first;
      ++m_size;
    }
    m_dummy.m_prev = prev;
    prev->m_next = &m_dummy;
  }

  template <typename... Args>
    requires std::constructible_from<ValueType, Args...>
  void _uninit_assign(usize n, Args &&...args) {
    ListNode *prev = &m_dummy;
    for (usize i = 0; i < n; ++i) {
      ListNode *node = newNode();
      prev->m_next = node;
      node->m_prev = prev;
      std::construct_at(&node->value(), forward<Args>(args)...);
      prev = node;
    }
    m_dummy.m_prev = prev;
    prev->m_next = &m_dummy;
    m_size = n;
  }
};

} // namespace mystl
