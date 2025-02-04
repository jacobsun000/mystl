module;

#include <stdexcept> // std::out_of_range

import type;
import utility;
import concepts;
import iterator;

export module array;

export namespace mystl {

template <class T, usize N> struct Array {
  T m_elements[N];

  using ValueType = T;

  T &operator[](usize i) noexcept { return m_elements[i]; }

  T const &operator[](usize i) const noexcept { return m_elements[i]; }

  T &at(usize i) {
    if (i >= N) [[__unlikely__]]
      throw std::out_of_range("array::at");
    return m_elements[i];
  }

  T const &at(usize i) const {
    if (i >= N) [[__unlikely__]]
      throw std::out_of_range("array::at");
    return m_elements[i];
  }

  void fill(T const &val) noexcept(std::is_nothrow_copy_assignable_v<T>) {
    fill_n(m_elements, N, val);
  }

  void swap(Array &that) noexcept(std::is_nothrow_swappable_v<T>) {
    for (usize i = 0; i < N; i++) {
      swap(m_elements[i], that.m_elements[i]);
    }
  }

  static constexpr bool empty() noexcept { return false; }

  static constexpr usize size() noexcept { return N; }

  T const *data() const noexcept { return m_elements; }

  T *data() noexcept { return m_elements; }

  T const *begin() const noexcept { return m_elements; }

  T const *end() const noexcept { return m_elements + N; }

  T *begin() noexcept { return m_elements; }

  T *end() noexcept { return m_elements + N; }

  Iterator<T> iter() const { return Iterator<T>(m_elements, N); }
};

template <class T> struct Array<T, 0> {
  using ValueType = T;

  T &operator[](usize i) noexcept {}

  T const &operator[](usize i) const noexcept {}

  T &at(usize i) { throw std::out_of_range("array::at"); }

  T const &at(usize i) const { throw std::out_of_range("array::at"); }

  void fill(T const &) noexcept {}

  void swap(Array &) noexcept {}

  static constexpr bool empty() noexcept { return true; }

  static constexpr usize size() noexcept { return 0; }

  T const *data() const noexcept { return null; }

  T *data() noexcept { return null; }

  T const *begin() const noexcept { return null; }

  T const *end() const noexcept { return null; }

  T *begin() noexcept { return null; }

  T *end() noexcept { return null; }

  Iterator<T> iter() const { return Iterator<T>(0); }
};

template <typename Tp, typename... Ts>
  requires(std::same_as<Tp, Ts> && ...)
Array(Tp, Ts...) -> Array<Tp, 1 + sizeof...(Ts)>;

}; // namespace mystl
