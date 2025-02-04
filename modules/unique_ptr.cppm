module;

#include <type_traits>
#include <utility>

import utility;
import type;

export module unique_ptr;

namespace _mystl_impl {
template <class Tp> struct DefaultDeleter {
  void operator()(Tp *p) const { delete p; }
};

template <class Tp> struct DefaultDeleter<Tp[]> {
  void operator()(Tp *p) const { delete[] p; }
};
} // namespace _mystl_impl

export namespace mystl {
template <class Tp, class Deleter = _mystl_impl::DefaultDeleter<Tp>>
struct UniquePtr {
private:
  Tp *p;
  [[no_unique_address]] Deleter deleter;

  template <class Up, class UDeleter> friend struct UniquePtr;

public:
  using element_type = Tp;
  using pointer = Tp *;
  using deleter_type = Deleter;

  UniquePtr(Nullptr = null) noexcept : p(null) {}

  explicit UniquePtr(Tp *p) noexcept : p(p) {}

  template <class Cp, class CDeleter>
    requires(std::convertible_to<Cp *, Tp *>)
  UniquePtr(UniquePtr<Cp, CDeleter> &&that) noexcept : p(that.p) {
    that.p = null;
  }

  ~UniquePtr() noexcept {
    if (p)
      deleter(p);
  }

  UniquePtr(UniquePtr const &that) = delete;
  UniquePtr &operator=(UniquePtr const &that) = delete;

  UniquePtr(UniquePtr &&that) noexcept : p(that.p) { that.p = null; }

  UniquePtr &operator=(UniquePtr &&that) noexcept {
    if (this == that) [[unlikely]] {
      return *this;
    }
    if (p)
      deleter(p);
    p = exchange(that.p, null);
  }

  void swap(UniquePtr &that) noexcept { swap(p, that.p); }

  Tp *get() const noexcept { return p; }

  Tp *operator->() const noexcept { return p; }

  std::add_lvalue_reference_t<Tp> operator*() const noexcept { return *p; }

  Deleter get_deleter() const noexcept { return deleter; }

  Tp *release() noexcept {
    Tp *result = p;
    p = null;
    return result;
  }

  void reset(Tp *ptr = null) noexcept {
    if (p)
      deleter(p);
    p = ptr;
  }

  explicit operator bool() const noexcept { return p != null; }

  bool operator==(UniquePtr const &that) const noexcept { return p == that.p; }

  bool operator!=(UniquePtr const &that) const noexcept { return p != that.p; }

  bool operator<(UniquePtr const &that) const noexcept { return p < that.p; }

  bool operator<=(UniquePtr const &that) const noexcept { return p <= that.p; }

  bool operator>(UniquePtr const &that) const noexcept { return p > that.p; }

  bool operator>=(UniquePtr const &that) const noexcept { return p >= that.p; }
};

template <class Tp, class _Deleter>
struct UniquePtr<Tp[], _Deleter> : UniquePtr<Tp, _Deleter> {
  using UniquePtr<Tp, _Deleter>::UniquePtr;

  std::add_lvalue_reference_t<Tp> operator[](usize i) { return this->get()[i]; }
};

template <typename Tp, typename... Args>
  requires(!std::is_unbounded_array_v<Tp>)
UniquePtr<Tp> makeUnique(Args &&...args) {
  return UniquePtr<Tp>(new Tp(forward<Args>(args)...));
}

template <typename Tp>
  requires(!std::is_unbounded_array_v<Tp>)
UniquePtr<Tp> makeUniqueForOverwrite() {
  return UniquePtr<Tp>(new Tp);
}

template <typename Tp>
  requires(std::is_unbounded_array_v<Tp>)
UniquePtr<Tp> makeUnique(std::size_t len) {
  return UniquePtr<Tp>(new std::remove_extent_t<Tp>[len]());
}

template <typename Tp>
  requires(std::is_unbounded_array_v<Tp>)
UniquePtr<Tp> makeUniqueForOverwrite(std::size_t len) {
  return UniquePtr<Tp>(new std::remove_extent_t<Tp>[len]);
}
} // namespace mystl
