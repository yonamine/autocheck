// RUN: autocheck -verify -Wpure-func-override %s

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
