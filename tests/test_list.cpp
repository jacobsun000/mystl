import list;
import type;
import utility;
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace mystl;

i32 main() {
  List<i32> arr;
  for (i32 i = 0; i < 16; i++) {
    printf("arr.push_back(%d)\n", i);
    arr.push_back(i);
  }

  for (auto it : arr) {
    printf("%d\n", it);
  }
}
