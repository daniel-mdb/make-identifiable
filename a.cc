#include <iostream>
#include <functional>
#include <source_location>

namespace {
class MustIdentify {
public:
  explicit MustIdentify() { }
  MustIdentify (const MustIdentify& f) { const_cast<MustIdentify&>(f)._identified = true; }
  ~MustIdentify() {
    if (!_identified) {
      std::cerr << "The function exited before calling into identify" << std::endl;
    }
  }
  void operator () (const std::source_location& l = std::source_location::current()) {
    _identified = true;
    printf("%s (%zu:%zu) %s\n", l.file_name(), l.line(), l.column(), l.function_name());
  }
private:
  bool _identified = false;
};

template <typename F, typename R, typename... A>
constexpr bool _no_throw(R(A...)) noexcept {
  return std::is_nothrow_invocable_v<F, A...>;
}
} // namespace

template<typename F>
constexpr bool no_throw(F f) noexcept {
  return _no_throw<F>(f);
}

namespace {
template <typename F, typename R, typename... A>
constexpr std::function<R(A...)> _make_identifiable(F&& f, R(A...)) {
  if (!no_throw(f)) {
    throw std::runtime_error("F must be declared noexcept or its first parameter must be of type MustIdentify");
  }
  return f;
}

template <typename F, typename R, typename... A>
constexpr std::function<R(A...)> _make_identifiable(F&& f, R(MustIdentify, A...)) {
  return std::bind_front(std::forward<F>(f), MustIdentify());
}
} // namespace

template<typename F>
auto make_identifiable(F&& f) {
  return _make_identifiable(std::forward<F>(f), f);
}

void a(int, char) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
} 

void b(int, char) noexcept {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void c(MustIdentify i, int, char) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void d(MustIdentify i, int, char) {
  i();
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main(void) {
  std::function<void(int, char)> z;
  try {
    /*
     * Function w/o noexcept without identify throws right away.
     */
    z = make_identifiable(a);
    z(1, 'a');
  } catch (const std::exception& e) {
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
  }

  try {
    /*
     * Function with noexcept passes.
     */
    z = make_identifiable(b);
    z(1, 'a');
  } catch (const std::exception& e) {
    std::cerr << "exception " << e.what() << std::endl;
  }

  try {
    /*
     * Function with MustIdentify passes, error is printed in case it does not call into the object.
     */
    z = make_identifiable(c);
    z(1, 'a');
  } catch (const std::exception& e) {
    std::cerr << "exception " << e.what() << std::endl;
  }

  try {
    /*
     * Function with MustIdentify passes and prints its location.
     */
    z = make_identifiable(d);
    z(1, 'a');
  } catch (const std::exception& e) {
    std::cerr << "exception " << e.what() << std::endl;
  }
  return 0;
}


