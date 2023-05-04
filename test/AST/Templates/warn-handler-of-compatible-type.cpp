// RUN: autocheck -verify -Whandler-of-compatible-type %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {

class A final {};
class B final {};
class C {};
class D final : public C {};

void Fn1(int32_t i) {
  try {
    if (i > 10) {
      throw A(); // Compliant
    } else {
      throw B(); // expected-warning {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
    }
  } catch (A const &) {
  }
}

void Fn2(int32_t i)  {
  try {
    if (i > 10) {
      throw D(); // Compliant
    }
  } catch (C const &) {
  }
}


void Fn3() {
  try {
    throw 12;
  } catch (int32_t const) {
    throw; // expected-warning {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
  }
}

void Fn4() {
  try {
    throw 2;
  } catch (...) { // Compliant
  }
}

  void Fn5() {
    try {
      try {
        throw 0; // expected-warning {{Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point}}
      } catch (const float) {
      }
    } catch (const char  *) {
    }
  }
}

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@9 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@12 1 {{Unused function 'Fn1'}}
// autosar-warning@23 1 {{Unused function 'Fn2'}}
// autosar-warning@33 1 {{Unused function 'Fn3'}}
// autosar-warning@41 1 {{Unused function 'Fn4'}}
// autosar-warning@48 1 {{Unused function 'Fn5'}}
// autosar-warning@54 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@54 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@9 1 {{class has implicit destructor}}
