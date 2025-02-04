export module utility;

import type;
import concepts;

export namespace mystl {

// Get the type of an expression
template <typename T> constexpr T &&declval() noexcept {
  return static_cast<T &&>(*static_cast<T *>(nullptr));
}

// Remove reference
template <typename T> struct remove_reference {
  using type = T;
};

template <typename T> struct remove_reference<T &> {
  using type = T;
};

template <typename T> struct remove_reference<T &&> {
  using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

// Remove const
template <typename T> struct remove_const {
  using type = T;
};

template <typename T> struct remove_const<const T> {
  using type = T;
};

// Remove const reference
template <typename T> struct remove_cvref {
  using type = typename remove_const<typename remove_reference<T>::type>::type;
};

template <typename T> using remove_cvref_t = typename remove_cvref<T>::type;

// Is Lvalue Reference
template <typename T> struct is_lvalue_reference : FalseType {};

template <typename T> struct is_lvalue_reference<T &> : TrueType {};

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

// move
template <typename T> constexpr remove_reference_t<T> &&move(T &&t) noexcept {
  return static_cast<remove_reference_t<T> &&>(t);
}

// forward
template <typename T> constexpr T &&forward(remove_reference_t<T> &t) noexcept {
  return static_cast<T &&>(t);
}

template <typename T>
constexpr T &&forward(remove_reference_t<T> &&t) noexcept {
  static_assert(!is_lvalue_reference_v<T>,
                "Invalid forward of an rvalue as an lvalue");
  return static_cast<T &&>(t);
}

// exchange
template <typename T, typename U = T> T exchange(T &obj, U &&new_value) {
  T old_value = move(obj);
  obj = forward<U>(new_value);
  return old_value;
}

// swap
template <typename T> void swap(T &a, T &b) noexcept {
  T temp = move(a);
  a = move(b);
  b = move(temp);
}

template <typename C1, typename C2>
  requires container<C1> && container<C2>
constexpr bool equal(C1 c1, C2 c2) {
  if (c1.size() != c2.size()) {
    return false;
  }
  auto first1 = c1.begin(), last1 = c1.end();
  auto first2 = c2.begin(), last2 = c2.end();

  for (; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2)) {
      return false;
    }
  }
  return true;
}

template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                     BinaryPredicate p) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!p(*first1, *first2)) {
      return false;
    }
  }
  return true;
}

// Copy N elements from source to destination
template <typename InputIt, typename OutputIt>
OutputIt copy_n(InputIt first, usize count, OutputIt result) {
  for (usize i = 0; i < count; ++i, ++first, ++result) {
    *result = *first;
  }
  return result;
}

// Fill N elements with a value
template <typename InputType, typename OutputIt>
OutputIt fill_n(OutputIt result, usize count, InputType value) {
  for (usize i = 0; i < count; ++i, ++result) {
    *result = value;
  }
  return result;
}

// Min function
template <typename T> const T &min(const T &a, const T &b) {
  return (a < b) ? a : b;
}

// Max function
template <typename T> const T &max(const T &a, const T &b) {
  return (a > b) ? a : b;
}

} // namespace mystl
