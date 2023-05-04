// RUN: autocheck -verify -Wvirtual-func-specifier %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class Foo1 {
public:
  virtual ~Foo1() {}
  void method1() {}
  virtual void method2() {}
  virtual void method3() {}
  virtual void method4() {}
  virtual void method5() {}
  virtual void method6() {}
  virtual void method7() {}
  virtual void method8() {}
  virtual void method9() {}

  virtual Foo1 &operator++() {
    return *this;
  }

  virtual Foo1 &operator--() {
    return *this;
  }
};

class Foo2 : public Foo1 {
public:
  virtual ~Foo2() override {} // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}

  virtual void method2() {}
  void method3() override {}
  void method4() final {}

  void method5() override final {}         // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
  virtual void method6() override {}       // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
  virtual void method7() final {}          // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
  virtual void method8() final override {} // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
  void method9() {}                        // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}

  virtual void method10() final = 0; // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
  virtual void method11() final {}   // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}

  Foo1 &operator++() override {
    return *this;
  }

  virtual Foo1 &operator--() override { // expected-warning {{Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final}}
    return *this;
  }
};
} // namespace

// autosar-warning@31 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
// autosar-warning@35 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
// autosar-warning@38 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
// autosar-warning@39 1 {{Each overriding virtual function shall be declared with the override or final specifier}}
