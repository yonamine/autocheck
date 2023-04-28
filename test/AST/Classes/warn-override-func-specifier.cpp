// RUN: autocheck -verify -Woverride-func-specifier %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class Foo1 {
public:
  virtual ~Foo1() {}
  virtual void method1() = 0;
  virtual void method2() {}
  virtual void method3() {}
  virtual void method4() {}
  virtual void method5() {}

  virtual Foo1 &operator++() {
    return *this;
  }
};

class Foo2 : public Foo1 {
public:
  ~Foo2() override {}

  void method1() {}         // expected-warning 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
  virtual void method2() {} // expected-warning 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
  void method3() override {}
  void method4() final {}
  void method5() override final {} // expected-warning 1 {{Each overriding virtual function shall be declared with the override or final specifier}}

  Foo2 &operator++() override {
    return *this;
  }
};

class Foo3 : public Foo1 {
public:
  ~Foo3() {} // expected-warning 1 {{Each overriding virtual function shall be declared with the override or final specifier}}

  void method1() override {}
  void method2() override {}
  void method3() override {}
  void method4() final {}
  void method5() final {}

  Foo3 &operator++() { // expected-warning 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
    return *this;
  }
};
} // namespace

// autosar-warning@23 1 {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
// autosar-warning@27 1 {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
// autosar-warning@36 1 {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
// autosar-warning@44 1 {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
