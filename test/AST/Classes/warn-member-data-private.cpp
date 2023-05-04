// RUN: autocheck -verify -Wmember-data-private %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <cstdint>
namespace {
class Foo1 {
public:
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;
};

class Foo2 {
public:
  std::int32_t x; // expected-warning 1 {{Member data in non-POD class types shall be private}}
protected:
  std::int32_t y; // expected-warning 1 {{Member data in non-POD class types shall be private}}
public:
  Foo2(std::int32_t x) : x(x) {}
};

class Foo3 {
public:
  std::int32_t x; // expected-warning 1 {{Member data in non-POD class types shall be private}}
protected:
  std::int32_t y; // expected-warning 1 {{Member data in non-POD class types shall be private}}
private:
  std::int32_t z;
};
} // namespace

// autosar-warning@13 1 {{All constructors that are callable with a single argument of fundamental type shall be declared explicit}}
// autosar-warning@4 1 {{There shall be no unused include directives:}}
// autosar-note@4 1 {{But one or more of it's own #include directives is used}}
// autosar-warning@6 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@13 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
// autosar-warning@22 1 {{If a public destructor of a class is non-virtual, then the class should be declared final}}
