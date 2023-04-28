// RUN: autocheck -verify -Wnull-thrown-explicitly %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <iostream>

namespace {
void test1() {
  std::int32_t i;

  switch (i) {
  case 1:
    throw 0;
  case 2:
    throw 0UL;
  case 3:
    throw 0x0;
  case 4:
    throw 00;
  case 5:
    throw 0b0;
  case 6:
    throw NULL; // expected-warning {{NULL shall not be thrown explicitly}}
  case 7:
    throw nullptr;
  default:
    throw 1;
  }
}
} // namespace

// autosar-warning@19 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@11 1 {{variable 'i' is uninitialized when used here}}
// autosar-note@9 1 {{initialize the variable 'i' to silence this warning}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@8 1 {{Unused function 'test1'}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@25 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@23 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@21 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@19 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@17 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@15 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
// autosar-warning@13 1 {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
