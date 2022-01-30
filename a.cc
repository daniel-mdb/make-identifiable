#include <iostream>
#include <functional>

#pragma mark from here on
#define identify(...) (__VA_ARGS__) { \
  std::cout << "Function called " << __func__ << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;

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

  a();

  [] identify(int a = 1) // {
    std::cout << "Good bye" << std::endl;
  }();

  return 0;
}
#pragma mark until here
