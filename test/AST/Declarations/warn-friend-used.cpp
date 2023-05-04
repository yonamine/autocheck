// RUN: autocheck -verify -Wfriend-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class B {};

class A {
private:
  friend void f1(); // expected-warning {{Friend declarations shall not be used}}
  friend class B;  // expected-warning {{Friend declarations shall not be used}}
};

void f2() {}

template <class T>
class C {
  void f3();
};

class D {
  template <class T>
  friend void C<T>::f3(); // expected-warning {{Friend declarations shall not be used}}
};
} // namespace

// autosar-warning@9 1 {{Unused function 'f1'}}
// autosar-warning@13 1 {{Unused function 'f2'}}
// autosar-warning@17 1 {{Unused private method 'f3'}}
// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@7 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@16 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@20 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
