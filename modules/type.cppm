module;
#include <cstddef>
#include <cstdint>

export module type;

export namespace mystl {
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;
using isize = std::ptrdiff_t;

struct FalseType {
  static constexpr bool value = false;
  using value_type = bool;
  constexpr operator value_type() const noexcept { return value; }
};

struct TrueType {
  static constexpr bool value = true;
  using value_type = bool;
  constexpr operator value_type() const noexcept { return value; }
};

struct Nullptr {
  template <typename T> constexpr operator T *() const noexcept {
    return nullptr;
  }

  template <typename C, typename T> constexpr operator T C::*() const noexcept {
    return nullptr;
  }

  constexpr bool operator==(const Nullptr &) const noexcept { return true; }

  constexpr bool operator!=(const Nullptr &) const noexcept { return false; }
};

constexpr Nullptr null{};
} // namespace mystl
