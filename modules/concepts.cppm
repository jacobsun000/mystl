module;

#include <concepts>
#include <iterator>
#include <type_traits>

export module concepts;

// Begin the module interface
export namespace mystl {
////////////////////////////////
// Basic Type Concepts
////////////////////////////////

// Concept for integral types
template <typename T>
concept integral = std::is_integral_v<T>;

// Concept for floating-point types
template <typename T>
concept floating_point = std::is_floating_point_v<T>;

// Concept for arithmetic types (integral or floating-point)
template <typename T>
concept arithmetic = integral<T> || floating_point<T>;

// Concept for signed types
template <typename T>
concept signed_type = requires {
  requires integral<T> || floating_point<T>;
  requires T(-1) < T(0); // Negative comparison for signedness
};

// Concept for unsigned types
template <typename T>
concept unsigned_type = requires {
  requires integral<T>;
  requires T(0) <= T(-1);
};

// Concept for default-constructible types
template <typename T>
concept default_constructible = requires { T(); };

// Concept for copy-constructible types
template <typename T>
concept copy_constructible = requires(T a) {
  T(a);
  { a = a } -> std::convertible_to<bool>;
};

// Concept for move-constructible types
template <typename T>
concept move_constructible = requires(T a) {
  T(std::move(a));
  { a = std::move(a) } -> std::convertible_to<bool>;
};

////////////////////////////////
// Container Concepts
////////////////////////////////

// Concept for containers
template <typename T>
concept container = requires(T a) {
  typename T::ValueType;
  typename T::SizeType;
  typename T::Iterator;

  { a.begin() } -> std::input_or_output_iterator;
  { a.end() } -> std::input_or_output_iterator;
  { a.size() } -> std::same_as<typename T::SizeType>;
};

// Concept for resizable containers (e.g., vector, deque)
template <typename T>
concept resizable_container =
    container<T> && requires(T a, typename T::SizeType n) { a.resize(n); };

////////////////////////////////
// Iterator Concepts
////////////////////////////////

// Concept for Iterable
template <typename T>
concept iterable = requires(T t) {
  { t.begin() };
  { t.end() };
  { t.size() };
};

////////////////////////////////
// Callable and Functional Concepts
////////////////////////////////

// Concept for callable objects
template <typename F, typename... Args>
concept callable = requires(F f, Args... args) {
  { f(args...) };
};

// Concept for predicates (functions returning bool)
template <typename F, typename... Args>
concept predicate = requires(F f, Args... args) {
  { f(args...) } -> std::convertible_to<bool>;
};

} // namespace mystl
