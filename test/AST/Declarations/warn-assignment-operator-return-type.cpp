// RUN: autocheck -verify -Wassignment-operator-return-type %s

namespace {
// Test from the Autosar document.
class A {
public:
  A &operator=(const A &) & { return *(this); }
};
// Test from the Autosar document.
class B {
public:
  const B & operator=(const B &) & // expected-warning {{An assignment operator shall return a reference to "this"}}
  {
    return *this;
  }
};
// Test from the Autosar document.
class C {
public:
  C operator=(const C &) & // expected-warning {{An assignment operator shall return a reference to "this"}}
  {
    return *this;
  }
};
// Test from the Autosar document.
class D {
public:
  D *operator=(const D &) & { return this; } // expected-warning {{An assignment operator shall return a reference to "this"}}
};
} // namespace
