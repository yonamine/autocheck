// RUN: autocheck -verify -Wfinal-class-destructor %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
class Foo1 { // expected-warning {{If a public destructor of a class is non-virtual, then the class should be declared final}}
public:
  Foo1() {}
  ~Foo1() {}

private:
  int x;
};

class Foo2 {
public:
  Foo2() {}
  virtual ~Foo2() {}
};

class Foo3 {
public:
  Foo3() {}

private:
  ~Foo3() {}
};

class Foo4 {
public:
  Foo4() {}

protected:
  ~Foo4() {}
};
} // namespace

// autosar-warning@11 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@25 1 {{Unused private method '~Foo3'}}
