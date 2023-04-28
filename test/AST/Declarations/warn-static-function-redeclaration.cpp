// RUN: autocheck -verify -Wstatic-function-redeclaration %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@7 1 {{Unused function 'func_1'}}
// autosar-warning@10 1 {{Unused function 'func_2'}}
// autosar-warning@14 1 {{Unused function 'func_3'}}
// autosar-warning@18 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
