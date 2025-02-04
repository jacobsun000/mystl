#include <iostream>

import vector;
import iterator;
import array;
import type;
import string;

using namespace mystl;

i32 main() {
  auto a = Array<i32, 10>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (auto &ai : a) {
    std::cout << ai << '\n';
  }

  auto b = a.iter()
               .map<i32>([](auto x) { return x * 2; })
               .filter([](auto x) { return x % 3 != 0; })
               .into<Vector<i32>>();

  for (auto &bi : b) {
    std::cout << bi << '\n';
  }
  auto c = "abcdefgHelloWorld"_m;
  auto d = c.iter().map<char>([](char x) { return x + 1; }).into<String>();
  for (auto &di : d) {
    std::cout << di;
  }
  std::cout << '\n';

  return 0;
}
