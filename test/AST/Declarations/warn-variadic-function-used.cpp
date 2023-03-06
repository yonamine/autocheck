// RUN: autocheck -verify -Wvariadic-function-used %s

#include <cstdint>
namespace {
void function1(std::int32_t a, ...) {} // expected-warning 1 {{Functions shall not be defined using the ellipsis notation}}

class Foo {
  void method1(std::int32_t a, ...) {} // expected-warning 1 {{Functions shall not be defined using the ellipsis notation}}

  static void function2(std::int32_t a, ...) {} // expected-warning 1 {{Functions shall not be defined using the ellipsis notation}}
};

template <typename Type1, typename... Type2>
void function3(const Type1 &a, const Type2 &... b) {}
} // namespace
