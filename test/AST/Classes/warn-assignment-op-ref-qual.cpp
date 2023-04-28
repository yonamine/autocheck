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
