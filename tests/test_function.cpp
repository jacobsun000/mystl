#include <cstdio>
#include <iostream>

import function;
import type;

using namespace mystl;

void func_hello(i32 i) { printf("#%d Hello\n", i); }

struct func_printnum_t {
  void operator()(i32 i) const { printf("#%d Numbers are: %d, %d\n", i, x, y); }
  i32 x;
  i32 y;
};

void repeattwice(Function<void(i32)> const &func) {
  func(1);
  func(2);
}

i32 main() {
  i32 x = 4;
  i32 y = 2;
  repeattwice([=](i32 i) { printf("#%d Numbers are: %d, %d\n", i, x, y); });
  func_printnum_t func_printnum{x, y};
  repeattwice(func_printnum);
  repeattwice(func_hello);

  Function<void(i32)> f{[](i32 i) { printf("i = %d\n", i); }};

  f(2);
  auto ff = f;
  ff(3);

  return 0;
}
