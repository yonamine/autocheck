// RUN: autocheck -verify -Wimplicitly-captured-var %s

#include <cstdint>
namespace {
  void test() {

    std::int32_t locVar{0};
    [&locVar](const std::int32_t parameter1) {
      locVar += parameter1;
    };

    const std::int32_t a{1};
    static std::int32_t b{0};
    [a]() {
      b += a;
    };

    [&](const std::int32_t parameter2) { // expected-warning 1 {{Variables shall not be implicitly captured in a lambda expression}}
      locVar += parameter2;
    };
  }

  class B {
  public:
    std::int32_t a;
    void method() {
      [&]() { a++; };
    };
  };
}
