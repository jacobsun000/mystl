module;

#include <functional>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>

export module function;

export namespace mystl {

template <class Fn> struct Function {
  static_assert(!std::is_same_v<Fn, Fn>, "Not a valid function signature");
};

template <class Ret, class... Args> struct Function<Ret(Args...)> {
private:
  struct FuncBase {
    virtual Ret call(Args... args) = 0;
    virtual std::unique_ptr<FuncBase> clone() const = 0;
    virtual std::type_info const &type() const = 0;
    virtual ~FuncBase() = default;
  };

  template <class Fn> struct FuncImpl : FuncBase {
    Fn m_fn;

    template <class... CArgs>
    explicit FuncImpl(std::in_place_t, CArgs &&...args)
        : m_fn(std::forward<CArgs>(args)...) {}

    Ret call(Args... args) override {
      return std::invoke(m_fn, std::forward<Args>(args)...);
    }

    std::unique_ptr<FuncBase> clone() const override {
      return std::make_unique<FuncImpl>(std::in_place, m_fn);
    }

    std::type_info const &type() const override { return typeid(Fn); }
  };

  std::unique_ptr<FuncBase> m_base;

public:
  Function() = default;
  Function(std::nullptr_t) noexcept : Function() {}

  template <typename Fn>
    requires std::invocable<Fn, Args...> && std::is_copy_constructible_v<Fn> &&
             (std::is_same_v<std::decay_t<Fn>, Function<Ret(Args...)>> == false)
  Function(Fn &&__f)
      : m_base(std::make_unique<FuncImpl<std::decay_t<Fn>>>(
            std::in_place, std::forward<Fn>(__f))) {}

  Function(Function &&) = default;
  Function &operator=(Function &&) = default;

  Function(Function const &that)
      : m_base(that.m_base ? that.m_base->clone() : nullptr) {}

  Function &operator=(Function const &that) {
    if (that.m_base)
      m_base = that.m_base->clone();
    else
      m_base = nullptr;
  }

  explicit operator bool() const noexcept { return m_base != nullptr; }

  bool operator==(std::nullptr_t) const noexcept { return m_base == nullptr; }

  bool operator!=(std::nullptr_t) const noexcept { return m_base != nullptr; }

  Ret operator()(Args... args) const {
    if (!m_base) [[unlikely]]
      throw std::bad_function_call();
    return m_base->call(std::forward<Args>(args)...);
  }

  std::type_info const &target_type() const noexcept {
    return m_base ? m_base->type() : typeid(void);
  }

  template <class Fn> Fn *target() const noexcept {
    return m_base && typeid(Fn) == m_base->type()
               ? std::addressof(static_cast<FuncImpl<Fn> *>(m_base.get())->m_fn)
               : nullptr;
  }

  void swap(Function &that) const noexcept { m_base.swap(that.m_base); }
};
} // namespace mystl
