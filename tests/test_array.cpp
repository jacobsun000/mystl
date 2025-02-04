#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

import array;
import type;

using namespace mystl;

template <class T, usize N> void iota(Array<T, N> &a) noexcept {
  T count = 0;
  for (auto &ai : a) {
    ai = count++;
  }
}

i32 main() {
  auto a = Array{2, 1, 0};
  for (auto &ai : a) {
    std::cout << ai << '\n';
  }
  iota(a);
  for (auto &ai : a) {
    std::cout << ai << '\n';
  }
  std::cout << "front:" << a[0] << '\n';
  std::cout << "back:" << a[a.size() - 1] << '\n';
  for (auto it = a.begin(); it != a.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << '\n';
  return 0;
}
