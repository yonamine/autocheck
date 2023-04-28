// RUN: autocheck -verify -Wvirtual-method-final-class %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class Foo1 {
public:
  virtual void method1() = 0;
  virtual void method2() {}
  void method3() {}
};

class Foo2 final : public Foo1 {
public:
  void method1() final {}
  void method2() override {} // expected-warning 1 {{Virtual functions shall not be introduced in a final class}}
  void method3() {}
  virtual void method4() {} // expected-warning 1 {{Virtual functions shall not be introduced in a final class}}
};
}

// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@12 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
