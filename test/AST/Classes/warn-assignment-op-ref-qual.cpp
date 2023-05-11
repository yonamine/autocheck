// RUN: autocheck -verify -Wassignment-op-ref-qual %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class A {
  double x;
  double y;

public:
  A &operator*=(const A &) & {
    return *this;
  }
};

class B {
  double x;
  double y;

public:
  B &operator=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator+=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator-=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator*=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator/=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator%=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator&=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator|=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator^=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator<<=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
  B &operator>>=(const B &) { // expected-warning {{Assignment operators should be declared with the ref-qualifier &}}
    return *this;
  }
};
} // namespace

// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@15 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@20 1 {{User-defined copy and move assignment operators should use user-defined no-throw swap function}}
// autosar-warning@10 1 {{Type '(anonymous namespace)::A' is "cheap to copy" and should be passed by value}}
// autosar-warning@20 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@23 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@26 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@29 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@32 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@35 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@38 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@41 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@44 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@47 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
// autosar-warning@50 1 {{Type '(anonymous namespace)::B' is "cheap to copy" and should be passed by value}}
