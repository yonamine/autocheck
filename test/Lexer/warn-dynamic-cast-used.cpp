// RUN: autocheck -verify -Wdynamic-cast-used %s

namespace {
class A {};
class B : public A {};

A *test() {
  B *b{new B{}};
  A *a = dynamic_cast<A *>(b); // expected-warning {{dynamic_cast should not be used}}
  return a;
}
} // namespace
