// RUN: autocheck -verify -Wnsdmi-and-ctor-init-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
#include <utility>

namespace {
// Autosar pdf
class A {
public:
  A() : i1{0}, i2{0} {
  }

private:
  std::int32_t i1;
  std::int32_t i2;
};
// Autosar pdf
class B {
private:
  std::int32_t i1{0};
  std::int32_t i2{0};
};
// Autosar pdf
class C {
public:
  C() : i2{0} { // expected-warning 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
  }

  C(C const &oth) : i1{oth.i1}, // compliant by exception
                    i2{oth.i2} {
  }

  C(C &&oth) : i1{std::move(oth.i1)}, // compliant by exception
               i2{std::move(oth.i2)} {
  }

private:
  std::int32_t i1{0}; // expected-note 1 {{Member already initialized by NSDMI here}}
  std::int32_t i2;
};
// Custom example
class D {
public:
  D() : i2{0} {
  }

private:
  std::int32_t i1;
  std::int32_t i2;
};
// Custom example: ALL member initializations should be conducted using the SAME syntax
// i2 is initialized using constructor initialization and i3 is initialised using NSDMI
class E {
public:
  E() : i2{0} { // expected-warning 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
  }

private:
  std::int32_t i1;
  std::int32_t i2;
  std::int32_t i3{0}; // expected-note 1 {{Member already initialized by NSDMI here}}
};
// Custom example: check only within one class
struct F {
  F() : i1{0}, i2{0} {
  }
  std::int32_t i1;
  std::int32_t i2;
};
struct G : public F {
  std::int32_t i1{0};
  std::int32_t i2{0};
};
// Custom example: check nested classes
class H {
public:
  H() : h1{2}, h3{3} { // expected-warning 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
  }

private:
  class I {
  public:
    I() : i1{2} { // expected-warning 1 {{Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type}}
    }

  private:
    std::int32_t i1;
    std::int32_t i2{2}; // expected-note 1 {{Member already initialized by NSDMI here}}
  };

  std::int32_t h1{3}; // expected-note 1 {{Member already initialized by NSDMI here}}
  std::int32_t h2;
  std::int32_t h3{34};
};
} // namespace

// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@5 1 {{There shall be no unused include directives:}}
// autosar-note@5 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@65 1 {{A non-POD type should be defined as class}}
// autosar-warning@68 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@69 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@71 1 {{A non-POD type should be defined as class}}
// autosar-warning@72 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@73 1 {{Member data in non-POD class types shall be private}}
// autosar-warning@9 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@19 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@43 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@54 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@76 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@82 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@65 1 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
// autosar-warning@71 1 {{A type defined as struct shall: (4) not inherit from another struct or class}}
// autosar-warning@71 1 {{Destructor of a base class shall be public virtual, public override or protected non-virtual}}
// autosar-note@65 1 {{class has implicit destructor}}
