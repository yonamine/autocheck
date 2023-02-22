// RUN: autocheck -verify -Wfinal-class-destructor %s

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
