// RUN: autocheck -verify -Wthrow-in-noexcept-func %s

#include <stdexcept>
namespace{
  void F1() noexcept { // expected-note {{function declared non-throwing here}}
      throw std::runtime_error("Error"); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
      // expected-note@-1 {{'F1' has a non-throwing exception specification but can still throw}}
  }
  void F2() noexcept(true) {
    try {
      throw std::runtime_error("Error");
    } catch (std::runtime_error &e) {
    }
  }
  void F3(float x) noexcept(true) { // expected-note {{function declared non-throwing here}}
      try {
          if(x > 0.0F) {
              throw std::runtime_error("Error"); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
              // expected-note@-1 {{'F3' has a non-throwing exception specification but can still throw}}
          }
          else {
              throw 0.0F;
          }
      } catch(float &y) {
      }
  }
  void F4() noexcept(false) {
      throw std::runtime_error("Error");
  }
  void F5() noexcept(true) { // expected-note {{function declared non-throwing here}}
    F4(); // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
    // expected-note@-1 {{Cannot call not noexcept function from the noexcept function}}
  }
  template <typename T>
  T F6(T x) noexcept(std::is_floating_point<T>::value) { // expected-note {{function declared non-throwing here}}
    F4();                                                // expected-warning {{If a function is declared to be noexcept, noexcept(true) or noexcept(<true condition>), then it shall not exit with an exception}}
                                                         // expected-note@-1 {{Cannot call not noexcept function from the noexcept function}}
    x += x;
    return x;
  }
}
int main() {
  F1();
  F2();
  F3(1.0F);
  F4();
  F5();
  F6<float>(1.0F);
  return 0;
}
