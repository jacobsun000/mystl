import type;
import utility;
import vector;

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace mystl;

i32 main() {
  Vector<i32> arr;
  for (i32 i = 0; i < 16; i++) {
    printf("arr.push(%d)\n", i);
    arr.push(i);
  }
  arr.insert(3, 40);
  arr.insert(3, 41);
  arr.insert(3, 42);
  for (usize i = 0; i < arr.size(); i++) {
    printf("arr[%zd] = %d\n", i, arr[i]);
  }

  Vector<i32> bar(3);
  printf("arr.size() = %zd\n", arr.size());
  printf("bar.size() = %zd\n", bar.size());
  bar = move(arr);
  printf("arr.size() = %zd\n", arr.size());
  printf("bar.size() = %zd\n", bar.size());
  printf("sizeof(Vector) = %zd\n", sizeof(Vector<i32>));
}
