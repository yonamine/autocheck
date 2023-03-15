// RUN: autocheck -verify -Wfriend-used %s

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
