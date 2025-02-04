module;
#include <stdexcept>
#include <utility>

import type;
import concepts;
import utility;

export module buffer;

export namespace mystl {

// TODO: Implement allocator
template <class T> struct Buffer {
protected:
  T *m_data = null;
  usize m_size = 0;
  usize m_capacity = 0;

public:
  Buffer() noexcept : m_data(null), m_size(0), m_capacity(0) {}

  template <class L>
    requires container<L> && (std::is_same_v<typename L::ValueType, T>)
  Buffer(L list) {
    reserve(list.size());
    copy_n(list.begin(), list.size(), begin());
    m_size = list.size();
  }

  Buffer(const T *data, usize n) {
    reserve(n);
    copy_n(data, n, m_data);
    m_size = n;
  }

  explicit Buffer(usize n) {
    m_data = new T[n];
    m_capacity = m_size = n;
    for (usize i = 0; i != n; i++) {
      std::construct_at(&m_data[i]);
    }
  }

  Buffer(usize n, T const &val) {
    m_data = new T[n];
    m_capacity = m_size = n;
    for (usize i = 0; i != n; i++) {
      std::construct_at(&m_data[i], val);
    }
  }

  Buffer(Buffer &&that) noexcept {
    m_data = that.m_data;
    m_size = that.m_size;
    m_capacity = that.m_capacity;
    that.m_data = null;
    that.m_size = 0;
    that.m_capacity = 0;
  }

  Buffer(Buffer const &that) {
    m_capacity = m_size = that.m_size;
    if (m_size != 0) {
      m_data = new T[m_size];
      for (usize i = 0; i != m_size; i++) {
        std::construct_at(&m_data[i], std::as_const(that.m_data[i]));
      }
    } else {
      m_data = null;
    }
  }

  Buffer &operator=(Buffer const &that) {
    if (&that == this) [[unlikely]]
      return *this;
    reserve(that.m_size);
    m_size = that.m_size;
    for (usize i = 0; i != m_size; i++) {
      std::construct_at(&m_data[i], std::as_const(that.m_data[i]));
    }
    return *this;
  }

  Buffer &operator=(Buffer &&that) noexcept {
    if (&that == this) [[unlikely]]
      return *this;
    for (usize i = 0; i != m_size; i++) {
      std::destroy_at(&m_data[i]);
    }
    if (m_capacity != 0) {
      delete[] m_data;
    }
    m_data = that.m_data;
    m_size = that.m_size;
    m_capacity = that.m_capacity;
    that.m_data = null;
    that.m_size = 0;
    that.m_capacity = 0;
    return *this;
  }

  void clear() noexcept {
    for (usize i = 0; i != m_size; i++) {
      std::destroy_at(&m_data[i]);
    }
    m_size = 0;
  }

  void resize(usize n) {
    if (n < m_size) {
      for (usize i = n; i != m_size; i++) {
        std::destroy_at(&m_data[i]);
      }
      m_size = n;
    } else if (n > m_size) {
      reserve(n);
      for (usize i = m_size; i != n; i++) {
        std::construct_at(&m_data[i]);
      }
    }
    m_size = n;
  }

  void resize(usize n, T const &val) {
    if (n < m_size) {
      for (usize i = n; i != m_size; i++) {
        std::destroy_at(&m_data[i]);
      }
      m_size = n;
    } else if (n > m_size) {
      reserve(n);
      for (usize i = m_size; i != n; i++) {
        std::construct_at(&m_data[i], val);
      }
    }
    m_size = n;
  }

  void shrink_to_fit() noexcept {
    auto old_data = m_data;
    auto old_cap = m_capacity;
    m_capacity = m_size;
    if (m_size == 0) {
      m_data = null;
    } else {
      m_data = new T[m_size];
    }
    if (old_cap == 0) [[unlikely]] {
      return;
    }
    for (usize i = 0; i != m_size; i++) {
      std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
      std::destroy_at(&old_data[i]);
    }
    delete[] old_data;
  }

  void reserve(usize n) {
    if (n <= m_capacity)
      return;
    n = max(n, m_capacity * 2);
    auto old_data = m_data;
    auto old_cap = m_capacity;
    if (n == 0) {
      m_data = null;
      m_capacity = 0;
    } else {
      m_data = new T[n];
      m_capacity = n;
    }
    if (old_cap != 0) {
      for (usize i = 0; i != m_size; i++) {
        std::construct_at(&m_data[i], std::move_if_noexcept(old_data[i]));
      }
      for (usize i = 0; i != m_size; i++) {
        std::destroy_at(&old_data[i]);
      }
      delete[] old_data;
    }
  }

  void erase(usize index, usize count = 1) {
    for (usize i = index; i != m_size; i++) {
      std::destroy_at(&m_data[i]);
      if (i + count < m_size) {
        std::construct_at(&m_data[i], std::move_if_noexcept(m_data[i + count]));
      }
    }
    m_size -= count;
  }

  usize capacity() const noexcept { return m_capacity; }

  usize size() const noexcept { return m_size; }

  bool empty() const noexcept { return m_size == 0; }

  T const &operator[](usize i) const noexcept { return m_data[i]; }

  T &operator[](usize i) noexcept { return m_data[i]; }

  T const &at(usize i) const {
    if (i >= m_size) [[unlikely]]
      throw std::out_of_range("vector::at");
    return m_data[i];
  }

  T &at(usize i) {
    if (i >= m_size) [[unlikely]]
      throw std::out_of_range("vector::at");
    return m_data[i];
  }

  void swap(Buffer &that) noexcept {
    swap(m_data, that.m_data);
    swap(m_size, that.m_size);
    swap(m_capacity, that.m_capacity);
  }

  void push(Buffer<T> other) {
    if (other.empty()) [[unlikely]]
      return;
    usize offset = other.size();
    if (m_size + offset >= m_capacity) [[unlikely]]
      reserve(m_size + offset);
    m_size += offset;
    copy_n(other.begin(), offset, begin() + m_size);
  }

  template <class... Args> void push(Args &&...args) {
    if (m_size + 1 >= m_capacity) [[unlikely]]
      reserve(m_size + 1);
    std::construct_at(&m_data[m_size], forward<Args>(args)...);
    m_size++;
  }

  template <class... Args> void insert(usize index, Args &&...args) {
    reserve(m_size + 1);
    for (usize i = m_size; i != index; i--) {
      std::construct_at(&m_data[i], move(m_data[i - 1]));
      std::destroy_at(&m_data[i - 1]);
    }
    m_size += 1;
    std::construct_at(&m_data[index], forward<Args>(args)...);
  }

  void insert(usize index, const Buffer &other) {
    usize offset = other.size();
    reserve(m_size + offset);
    for (usize i = m_size; i != index; i--) {
      std::construct_at(&m_data[i + offset], move(m_data[i]));
      std::destroy_at(&m_data[i]);
    }
    m_size += offset;
    copy_n(other.begin(), offset, begin() + index);
  }

  T pop() noexcept {
    m_size -= 1;
    return move(m_data[m_size]);
  }

  T *data() noexcept { return m_data; }

  T const *data() const noexcept { return m_data; }

  T *begin() noexcept { return m_data; }

  T *end() noexcept { return m_data + m_size; }

  T const *begin() const noexcept { return m_data; }

  T const *end() const noexcept { return m_data + m_size; }

  bool operator!() { return m_size == 0; }

  explicit operator bool() { return m_size != 0; }

  Buffer &operator+=(const Buffer &other) {
    push(other);
    return *this;
  }

  bool operator==(const Buffer &that) const noexcept {
    return std::equal(this->begin(), this->end(), that.begin(), that.end());
  }

  auto operator<=>(const Buffer &that) const noexcept {
    return std::lexicographical_compare_three_way(this->begin(), this->end(),
                                                  that.begin(), that.end());
  }

  ~Buffer() noexcept {
    for (usize i = 0; i != m_size; i++) {
      std::destroy_at(&m_data[i]);
    }
    if (m_capacity != 0) {
      delete[] m_data;
    }
  }
};
}; // namespace mystl
