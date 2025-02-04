// cstring.cppm
export module cstring;

import type;

export namespace mystl {

class CString {
public:
  using SizeType = usize;

  // Get the length of a C-style string
  static SizeType length(const char *str) noexcept {
    if (!str)
      return 0;
    const char *end = str;
    while (*end)
      ++end;
    return end - str;
  }

  // Compare two C-style strings lexicographically
  static int compare(const char *lhs, const char *rhs) noexcept {
    if (!lhs || !rhs) {
      return (lhs == rhs) ? 0 : (lhs ? 1 : -1);
    }
    while (*lhs && (*lhs == *rhs)) {
      ++lhs;
      ++rhs;
    }
    return (unsigned char)*lhs - (unsigned char)*rhs;
  }

  // Copy n characters from src to dest
  static char *copy(char *dest, const char *src, SizeType n) noexcept {
    if (!dest || !src || n == 0)
      return dest;
    char *dest_start = dest;
    while (n-- && (*dest++ = *src++))
      ;
    return dest_start;
  }

  // Concatenate two C-style strings
  static char *concatenate(char *dest, const char *src) noexcept {
    if (!dest || !src)
      return dest;
    char *dest_end = dest + length(dest);
    while ((*dest_end++ = *src++))
      ;
    return dest;
  }

  // Find the first occurrence of a character in a C-style string
  static const char *find(const char *str, char ch) noexcept {
    if (!str)
      return nullptr;
    while (*str) {
      if (*str == ch)
        return str;
      ++str;
    }
    return nullptr;
  }

  // Find the last occurrence of a character in a C-style string
  static const char *rfind(const char *str, char ch) noexcept {
    if (!str)
      return nullptr;
    const char *last = nullptr;
    while (*str) {
      if (*str == ch)
        last = str;
      ++str;
    }
    return last;
  }

  // Check if a C-style string starts with a given prefix
  static bool starts_with(const char *str, const char *prefix) noexcept {
    if (!str || !prefix)
      return false;
    while (*prefix) {
      if (*str++ != *prefix++)
        return false;
    }
    return true;
  }

  // Check if a C-style string ends with a given suffix
  static bool ends_with(const char *str, const char *suffix) noexcept {
    if (!str || !suffix)
      return false;
    SizeType str_len = length(str);
    SizeType suffix_len = length(suffix);
    if (suffix_len > str_len)
      return false;
    return compare(str + str_len - suffix_len, suffix) == 0;
  }
};
} // namespace mystl
