// RUN: autocheck -verify -Wexception-object-is-pointer %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class A {};

void test() {
  A a1{};
  A &a2{a1};
  A *a3{new A};

  int i;
  if (i == 0) {
    throw;
  } else if (i == 1) {
    throw a1;
  } else if (i == 2) {
    throw A();
  } else if (i == 3) {
    throw a2;
  } else if (i == 4) {
    throw &a1; // expected-warning {{An exception object shall not be a pointer}}
  } else if (i == 5) {
    throw a3; // expected-warning {{An exception object shall not be a pointer}}
  } else if (i == 6) {
    throw(*a3);
  } else {
    throw new A; // expected-warning {{An exception object shall not be a pointer}}
  }
}
} // namespace

// autosar-warning@12 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@13 1 {{variable 'i' is uninitialized when used here}}
// autosar-note@12 1 {{initialize the variable 'i' to silence this warning}}
// autosar-warning@7 1 {{Unused function 'test'}}
// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@12 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@9 1 {{An identifier declared to be an object or type shall be defined in a block that minimizes its visibility}}
// autosar-warning@28 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@26 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@24 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@22 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@20 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@18 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@16 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@14 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
