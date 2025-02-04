# 🌟 mystl — A Modern C++ STL Reimplementation

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)

🚀 **mystl** is a modern reimplementation of STL containers, concepts,
and algorithms in C++20, built from scratch with a focus on expressiveness,
performance, and usability.

## 📌 Features

✅ **Containers**: Custom implementations of arrays, vectors, and more!
✅ **Functional Programming Support**: `map`, `filter`, `fold`,
and `into` methods for expressive pipelines.
✅ **String Literals**: Custom string support with `"abc"_m` syntax.
✅ **C++20 Modules**: Uses modern module-based architecture for better maintainability.
✅ **Interoperability**: Works seamlessly with existing STL components.

---

## ⚡ Quick Start

```cpp
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
```

---

## 🔧 Installation

Build dependencies:

- [xmake](https://xmake.io/#/)

```sh
git clone https://github.com/jacobsun000/mystl.git
cd mystl
xmake
```

---

## 🎯 Roadmap

- [ ] 📦 Implement more STL containers (e.g., `Set`, `Map`, `Deque`)
- [ ] 🚀 Add multi-threading support
- [ ] 🛠 Improve functional programming utilities
- [ ] 📜 Extend documentation and add more examples

---

## 🤝 Contributing

We welcome contributions! Feel free to fork the repo, open an issue,
or submit a PR.
🚀 Let's build an awesome modern STL together!

---

## 📜 License

MIT License. See [LICENSE](./LICENSE) for details.

---

⭐ **If you like this project, don't forget to give it a star!** ⭐
