// RUN: autocheck -verify -Whandler-of-compatible-type %s

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

