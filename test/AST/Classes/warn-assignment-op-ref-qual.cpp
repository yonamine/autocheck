// RUN: autocheck -verify -Wassignment-op-ref-qual %s

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
