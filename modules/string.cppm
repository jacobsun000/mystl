module;

#include <cstdlib>
#include <string>

import type;
import buffer;
import cstring;
import iterator;

export module string;

export namespace mystl {

class String : public Buffer<char> {
public:
  String() : Buffer<char>(0) {}

  String(const char *data) {
    if (data == null) {
      Buffer<char>(0);
      return;
    }
    Buffer<char>(data, CString::length(data));
  }

  String(const char *data, usize length) : Buffer<char>(data, length) {}

  template <typename T> explicit String(T value) {
    std::string str_value = std::to_string(value);
    Buffer<char>(str_value.c_str(), str_value.length());
  }

  explicit String(bool value) {
    if (value) {
      Buffer<char>("True", 4);
    } else {
      Buffer<char>("False", 5);
    }
  }

  const char *c_str() {
    reserve(size() + 1);
    m_data[m_size] = '\0';
    return m_data;
  }

  bool operator==(const String &other) const {
    return Buffer<char>::operator==(other);
  }

  bool operator==(const char *other) const { return *this == String(other); }

  bool operator!=(const char *other) const { return !(*this == other); }

  template <typename T> String &operator+=(T const &value) {
    return *this += String(value);
  }

  String friend operator+(String const &left, String const &right);

  String friend operator+(String &&left, String const &right);

  template <typename T>
  String friend operator+(String const &left, T const &right);

  template <typename T>
  String friend operator+(T const &left, const String &right);

  template <typename T> String friend operator+(String &&left, T const &right);

  Iterator<char> iter() const { return Iterator<char>(m_data, m_size); }
};

String operator+(String const &left, String const &right) {
  usize new_size = left.size() + right.size();
  String s;
  s.reserve(new_size);
  s.push(Buffer<char>(left));
  s.push(Buffer<char>(right));
  return s;
}

String operator+(String &&left, String const &right) {
  left.push(Buffer<char>(right));
  return std::move(left);
}

template <typename T>
inline String operator+(String const &left, T const &right) {
  return left + String(right);
}

template <typename T>
inline String operator+(T const &left, String const &right) {
  return String(left) + right;
}

template <typename T> inline String operator+(String &&left, T const &right) {
  return std::forward<String>(left) + String(right);
}

inline String operator""_m(const char *data, usize size) {
  return String(data, size);
}

}; // namespace mystl
