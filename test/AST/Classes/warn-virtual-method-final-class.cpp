// RUN: autocheck -verify -Wvirtual-method-final-class %s

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
