// RUN: autocheck -verify -Wpure-func-override %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class Foo1 {
public:
  virtual void pureMethod1() = 0;
};

class Foo2 : public Foo1 {
public:
  void pureMethod1() override {}
  virtual void pureMethod2() = 0;
};

class Foo3 : public Foo2 {
public:
  void pureMethod1() override = 0; // expected-warning 1 {{A virtual function shall only be overridden by a pure virtual function if it is itself declared as pure virtual}}
  void pureMethod2() override = 0;
};
}

// autosar-warning@5 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@16 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@10 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@5 1 {{class has implicit destructor}}
// autosar-warning@16 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@10 1 {{class has implicit destructor}}
