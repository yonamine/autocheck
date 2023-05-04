// RUN: autocheck -verify -Wassignment-operator-return-type %s
// RUN: autocheck -verify=expected,autosar -Wall %s

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

// autosar-warning@8 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@13 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@21 1 {{Type '(anonymous namespace)::C' is "cheap to copy" and should be passed by value}}
// autosar-warning@29 1 {{Type '(anonymous namespace)::D' is "cheap to copy" and should be passed by value}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@11 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@19 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@27 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@8 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@13 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@21 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@29 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@8 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
// autosar-warning@29 1 {{Each expression statement and identifier declaration shall be placed on a separate line}}
