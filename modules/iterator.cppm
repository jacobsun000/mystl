export module iterator;

import function;
import concepts;
import utility;
import buffer;
import type;

export namespace mystl {

// TODO: Implement container-specific iterator
// Current implementation is very inefficient
template <class T> struct Iterator {
private:
  Buffer<T> m_data;
  usize m_index = 0;

public:
  template <typename... Args>
  Iterator(Args &&...args) : m_data(forward<Args>(args)...), m_index(0) {}

  bool has_next() const { return m_index < m_data.size(); };

  T next() { return m_data[m_index++]; };

  void for_each(Function<void(T &)> func) {
    for (usize i = 0; i != m_data.size(); i++) {
      func(m_data[i]);
    }
  }

  template <typename U> Iterator<U> map(Function<U(const T &)> func) const {
    Iterator<U> result(m_data.size());
    for (usize i = 0; i != m_data.size(); i++) {
      result.m_data.push(func(m_data[i]));
    }
    return result;
  }

  Iterator<T> filter(Function<bool(const T &)> func) const {
    Iterator<T> result;
    for (usize i = 0; i != m_data.size(); i++) {
      if (func(m_data[i])) {
        result.m_data.push(m_data[i]);
      }
    }
    return result;
  };

  template <typename U>
  U fold(U init, Function<U(const U &, const T &)> func) const {
    for (usize i = 0; i != m_data.size(); i++) {
      init = func(init, m_data[i]);
    }
    return init;
  };

  template <class Container>
    requires requires(Container c, T val) { c.push(val); }
  Container into() {
    Container result;
    while (this->has_next()) {
      T val = this->next();
      result.push(val);
    }
    return result;
  }
};

}; // namespace mystl
