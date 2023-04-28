// RUN: autocheck -verify -Wexception-handler-inversion %s
// RUN: autocheck -verify=expected,autosar -Wall %s
// Taken from clang/test/SemaCXX/exceptions.cpp, HandlerInversion namespace

namespace HandlerInversion {
struct B {};
struct D : B {};
struct D2 : D {};

void f1() {
  try {
  } catch (B &b) { // expected-note {{previous handler was here}}
  } catch (D &d) { // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}

void f2() {
  try {
  } catch (B *b) { // expected-note {{previous handler was here}}
  } catch (D *d) { // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}

void f3() {
  try {
  } catch (D &d) { // Ok
  } catch (B &b) {
  }
}

void f4() {
  try {
  } catch (B &b) { // Ok
  }
}

void f5() {
  try {
  } catch (int) {
  } catch (float) {
  }
}

void f6() {
  try {
  } catch (B &b) {  // expected-note {{previous handler was here}}
  } catch (D2 &d) {  // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}

void f7() {
  try {
  } catch (B *b) { // Ok
  } catch (D &d) { // Ok
  }

  try {
  } catch (B b) { // Ok
  } catch (D *d) { // Ok
  }
}

void f8() {
  try {
  } catch (const B &b) {  // expected-note {{previous handler was here}}
  } catch (D2 &d) {  // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }

  try {
  } catch (B &b) {  // expected-note {{previous handler was here}}
  } catch (const D2 &d) {  // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }

  try {
  } catch (B b) { // expected-note {{previous handler was here}}
  } catch (D &d) { // expected-warning {{Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class}}
  }
}
} // namespace HandlerInversion

// autosar-warning@39 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@7 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@6 {{class has implicit destructor}}
// autosar-warning@8 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@7 {{class has implicit destructor}}
// autosar-warning@7 {{A type defined as struct shall: (4) not inherit from another struct or class}}
// autosar-warning@8 {{A type defined as struct shall: (4) not inherit from another struct or class}}
// autosar-warning@7 {{A non-POD type should be defined as class}}
// autosar-warning@8 {{A non-POD type should be defined as class}}
// autosar-warning@12 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@13 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@19 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@20 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@26 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@27 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@33 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@39 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@40 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@46 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@47 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@53 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@54 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@58 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@59 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@66 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@70 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@75 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@76 {{Constexpr or const specifiers shall be used for immutable data declaration}}
