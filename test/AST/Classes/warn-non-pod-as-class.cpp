// RUN: autocheck -verify -Wnon-pod-as-class %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class Foo1 {
public:
  std::int32_t x;
  std::int32_t y;
};

struct Foo2 {
  std::int32_t x;
  std::int32_t y;
};

struct Foo3 { // expected-warning 1 {{A non-POD type should be defined as class}}
  const static std::int32_t y{0};
  Foo3(std::int32_t z) : x(z) {}

private:
  std::int32_t x;
};

class Foo4 {
  std::int32_t x;

public:
  const static std::int32_t y{0};
  Foo4(std::int32_t z) : x(z) {}
};
} // namespace

// autosar-warning@17 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@25 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@18 1 {{Unused variable 'y'}}
// autosar-warning@29 1 {{Unused variable 'y'}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@25 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@18 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@29 1 {{CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name}}
// autosar-warning@17 1 {{A type defined as struct shall: (2) not provide any special member functions or methods}}
