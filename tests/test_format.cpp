#include <print>

import vector;
import iterator;
import array;
import type;
import string;
import format;

using namespace mystl;

i32 main() {
  auto a = Array<i32, 10>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::println("a: {}", a);
  // a: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

  auto b = a.iter()
               .map<i32>([](auto x) { return x * 2; })
               .filter([](auto x) { return x % 3 != 0; })
               .into<Vector<i32>>();
  std::println("b: {}", b);
  // b: [2, 4, 8, 10, 14, 16, 20]

  auto c = "abcdefgHelloWorld"_m;
  std::println("c: {}", c);
  // c: abcdefgHelloWorld

  return 0;
}
