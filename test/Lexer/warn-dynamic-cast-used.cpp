// RUN: autocheck -verify -Wdynamic-cast-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class A {};
class B : public A {};

A *test() {
  B *b{new B{}};
  A *a{dynamic_cast<A *>(b)}; // expected-warning {{dynamic_cast should not be used}}
  return a;
}
} // namespace

// autosar-warning@8 1 {{Unused function 'test'}}
// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@9 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@10 1 {{Constexpr or const specifiers shall be used for immutable data declaration}}
// autosar-warning@6 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 {{class has implicit destructor}}
