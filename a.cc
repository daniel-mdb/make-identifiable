#include <iostream>
#include <functional>

#define identify(...) (MUST_IDENTIFY, __VA_ARGS__) { \
  std::cout << "Function called " << __func__ << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;

#pragma mark from here on
namespace {
  struct MUST_IDENTIFY {
    explicit MUST_IDENTIFY() { }
  };

  template<typename F, typename ... Args>
  auto make_identifiable_function(F && f, Args && ... args) noexcept {
    return std::bind_front(f, MUST_IDENTIFY(), std::forward<Args>(args)...);
  }

#if 0
  template<class Res, class ... ArgTypes>
  using identifiable_function = std::function<Res(MUST_IDENTIFY, ArgTypes...)>;
#endif

#if 0
  template<typename Res, typename ... ArgTypes>
  class identifiable_function : public std::function<Res(ArgTypes...)> {
  private:
    typedef std::function<Res(MUST_IDENTIFY, ArgTypes...)> BASE;
  public:
    Res operator () (ArgTypes ... args) const {
      return BASE::operator()(MUST_IDENTIFY(), args...);
    }
  };
#endif

}

void a identify(int a = 0) // {
  std::cout << "Hello World!" << std::endl;
}

#if 0
/*
 * the above macro would read nicer if the compiler could simply concatenate root
 * level scopes like in:
 * int b(void) { /* preamble */ } { return 0; }
 */
#endif

int main() {
  try {
    std::function<void(int)> a = a;
    a(1);
  } catch (const std::bad_function_call &e) {
    std::cerr << "error " << e.what() << std::endl;
  }

  try {
    // it would have been better to have a specialized type to enforce this.
    // Deserves more investigation!
    const auto b = make_identifiable_function(a);
    b(1);
  } catch (const std::bad_function_call &e) {
    std::cerr << "error " << e.what() << std::endl;
  }

  {
    const auto c = make_identifiable_function([] identify(int a = 1) // {
      std::cout << "Good bye" << std::endl;
    });

    c();
  }

  return 0;
}
#pragma mark until here
