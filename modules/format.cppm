module;

#include <print>
#include <ranges>

import concepts;
import array;
import string;
export module format;

template <mystl::iterable Container> struct std::formatter<Container> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Container &container, std::format_context &ctx) const {
    std::format_to(ctx.out(), "[");
    if (container.size() > 0) {
      std::format_to(ctx.out(), "{}", *container.begin());
    }
    for (auto it = std::next(container.begin()); it != container.end(); ++it) {
      std::format_to(ctx.out(), ", {}", *it);
    }
    return std::format_to(ctx.out(), "]");
  }
};

template <> struct std::formatter<mystl::String> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(mystl::String &str, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{}", str.c_str());
  }
};
