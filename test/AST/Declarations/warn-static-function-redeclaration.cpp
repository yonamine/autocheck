// RUN: autocheck -verify -Wstatic-function-redeclaration %s

#include <iostream>
namespace {
static void func_1();
void func_1() {} // expected-warning {{If a function has internal linkage then all re-declarations shall include the static storage class specifier}}

static void func_2();
static void func_2() {}

static void func_3();
void func_3(); // expected-warning {{If a function has internal linkage then all re-declarations shall include the static storage class specifier}}
static void func_3() {}
} // namespace

namespace test {
class A {
  public:
  static int32_t a(int32_t i);
};

int32_t A::a(int32_t i) {
  return i + 2;
}
} // namespace test

int32_t main() {
  std::cout << test::A::a(4) << std::endl;
  return 0;
}
