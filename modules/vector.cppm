export module vector;

import buffer;
import type;
import iterator;

export namespace mystl {

template <class T> struct Vector : public Buffer<T> {
  Vector() noexcept : Buffer<T>() {}

  explicit Vector(usize n) : Buffer<T>(n) {}

  Iterator<T> iter() const { return Iterator<T>(this); }
};

}; // namespace mystl
