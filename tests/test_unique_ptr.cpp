#include <cstdio>
#include <iostream>
#include <vector>

import unique_ptr;
import type;
using namespace mystl;

struct MyClass {
  i32 a, b, c;
};

struct Animal {
  virtual void speak() = 0;
  virtual ~Animal() = default;
};

struct Dog : Animal {
  i32 age;

  Dog(i32 age_) : age(age_) {}

  virtual void speak() { printf("Bark! I'm %d Year Old!\n", age); }
};

struct Cat : Animal {
  i32 &age;

  Cat(i32 &age_) : age(age_) {}

  virtual void speak() { printf("Meow! I'm %d Year Old!\n", age); }
};

i32 main() {
  std::vector<UniquePtr<Animal>> zoo;
  i32 age = 3;
  zoo.push_back(makeUnique<Cat>(age));
  zoo.push_back(makeUnique<Dog>(age));
  for (auto const &a : zoo) {
    a->speak();
  }
  age++;
  for (auto const &a : zoo) {
    a->speak();
  }
  return 0;
}
